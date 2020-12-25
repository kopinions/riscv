#pragma once
#include <scv.h>
#include <tlm.h>

#include <array>

#include "recordable_data.hpp"
#include "recording_extension.hpp"

enum tx_relationship {
  PARENT_CHILD = 0,     /*!< indicates parent child relationship */
  PREDECESSOR_SUCCESSOR /*!< indicates predecessor successor relationship */
};

static constexpr std::array<std::string_view, 2> tx_relationship_literal = {{"PARENT/CHILD", "PRED/SUCC"}};
static inline constexpr const char* relationship_name(tx_relationship rel) {
  return (tx_relationship_literal[rel].data());
}

template <typename TYPES = tlm::tlm_base_protocol_types>
class tlm_recordable_payload : public TYPES::tlm_payload_type {
 public:
  scv_tr_handle parent;
  uint64 id;
  tlm_recordable_payload& operator=(const typename TYPES::tlm_payload_type& x) {
    id = reinterpret_cast<uintptr_t>(&x);
    this->set_command(x.get_command());
    this->set_address(x.get_address());
    this->set_data_ptr(nullptr);
    this->set_data_length(x.get_data_length());
    this->set_response_status(x.get_response_status());
    this->set_byte_enable_ptr(nullptr);
    this->set_byte_enable_length(x.get_byte_enable_length());
    this->set_streaming_width(x.get_streaming_width());
    return (*this);
  }

  explicit tlm_recordable_payload(tlm::tlm_mm_interface* mm) : TYPES::tlm_payload_type(mm), parent(), id(0) {}
};

template <typename TYPES = tlm::tlm_base_protocol_types>
struct tlm_recording_types {
  using tlm_payload_type = tlm_recordable_payload<TYPES>;
  using tlm_phase_type = typename TYPES::tlm_phase_type;
};

template <typename TYPES = tlm::tlm_base_protocol_types>
class recorder : public virtual tlm::tlm_fw_transport_if<TYPES>,
                 public virtual tlm::tlm_bw_transport_if<TYPES>,
                 public virtual sc_core::sc_object {
 public:
  using recording_types = tlm_recording_types<TYPES>;
  using mm = payload_memory_manager<recording_types>;
  using recording_payload_type = typename recording_types::tlm_payload_type;
  recorder(sc_core::sc_port_b<tlm::tlm_fw_transport_if<TYPES>>& fw_port,
           sc_core::sc_port_b<tlm::tlm_bw_transport_if<TYPES>>& bw_port, bool recording_enabled = true,
           scv_tr_db* tr_db = scv_tr_db::get_default_db());

  recorder(const char* name, sc_core::sc_port_b<tlm::tlm_fw_transport_if<TYPES>>& fw_port,
           sc_core::sc_port_b<tlm::tlm_bw_transport_if<TYPES>>& bw_port, bool recording_enabled = true,
           scv_tr_db* tr_db = scv_tr_db::get_default_db());

  tlm::tlm_sync_enum nb_transport_bw(typename TYPES::tlm_payload_type&, typename TYPES::tlm_phase_type&,
                                     sc_core::sc_time&) override;

  tlm::tlm_sync_enum nb_transport_fw(typename TYPES::tlm_payload_type&, typename TYPES::tlm_phase_type&,
                                     sc_core::sc_time&) override;

  void b_transport(typename TYPES::tlm_payload_type&, sc_core::sc_time& t) override;

  void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range) override;

  bool get_direct_mem_ptr(typename TYPES::tlm_payload_type&, tlm::tlm_dmi& dmi_data) override;

  unsigned int transport_dbg(typename TYPES::tlm_payload_type&) override;

  bool enabled() const;

 private:
  sc_core::sc_port_b<tlm::tlm_fw_transport_if<TYPES>>& fw_port;
  sc_core::sc_port_b<tlm::tlm_bw_transport_if<TYPES>>& bw_port;
  sc_core::sc_attribute<bool> tracing_enabled;
  sc_core::sc_attribute<bool> timed_enabled;

  //! transaction generator handle for forward non-blocking transactions
  std::vector<scv_tr_generator<std::string, tlm::tlm_sync_enum>*> nb_fw_transactor;
  //! transaction generator handle for backward non-blocking transactions
  std::vector<scv_tr_generator<std::string, tlm::tlm_sync_enum>*> nb_bw_transactor;

  std::string m_base_name;
  scv_tr_db* m_db;
};

template <typename TYPES>
tlm::tlm_sync_enum recorder<TYPES>::nb_transport_bw(typename TYPES::tlm_payload_type& trans,
                                                    typename TYPES::tlm_phase_type& phase, sc_core::sc_time& t) {
  return tlm::TLM_UPDATED;
}

template <typename TYPES>
tlm::tlm_sync_enum recorder<TYPES>::nb_transport_fw(typename TYPES::tlm_payload_type& payload,
                                                    typename TYPES::tlm_phase_type& phase, sc_core::sc_time& t) {
  if (!enabled()) {
    return fw_port->nb_transport_fw(payload, phase, t);
  }

  scv_tr_handle h = nb_fw_transactor[payload.get_command()]->begin_transaction(std::string{phase.get_name()});
  if (timed_enabled.value) {
    auto *req = singleton<mm>::get().alloc();
    req->acquire();
    (*req) = payload;
    req->parent = h;
//    nb_timed_peq.notify(*req, phase, delay);
  }
  auto prev = payload.template get_extension<recording_extension>();
  if (prev == nullptr) {  // we are the first recording this transaction
    prev = new recording_extension(h, this);
    if (payload.has_mm())
      payload.set_auto_extension(prev);
    else
      payload.set_extension(prev);
  } else {
    h.add_relation(relationship_name(PREDECESSOR_SUCCESSOR), prev->tx());
  }
  // update the extension
  if (prev) {
    prev->tx(h);
  }
  h.record_attribute("delay", t.to_string());

  tlm::tlm_sync_enum status = fw_port->nb_transport_fw(payload, phase, t);
  h.record_attribute("trans", recordable_data{payload});
  h.record_attribute("tlm_phase[return_path]", std::string{phase.get_name()});
  h.record_attribute("delay[return_path]", t.to_string());
  // get the extension and free the memory if it was mine
  if (status == tlm::TLM_COMPLETED || (status == tlm::TLM_ACCEPTED && phase == tlm::END_RESP)) {
    payload.get_extension(prev);
    if (prev && prev->created_by() == this) {
      payload.set_extension(static_cast<recording_extension*>(nullptr));
      if (!payload.has_mm()) {
        delete prev;
      }
    }
    /*************************************************************************
     * do the timed notification if req. finished here
     *************************************************************************/
    //    if (timed_enabled.value) {
    //      tlm_recording_payload* req = mm::get().allocate();
    //      req->acquire();
    //      (*req) = payload;
    //      req->parent = h;
    //      nb_timed_peq.notify(*req, (status == tlm::TLM_COMPLETED && phase == tlm::BEGIN_REQ) ? tlm::END_RESP : phase,
    //                          t);
    //    }
  }
  // else if (timed_enabled.value && status == tlm::TLM_UPDATED) {
  //    tlm_recording_payload* req = mm::get().allocate();
  //    req->acquire();
  //    (*req) = trans;
  //    req->parent = h;
  //    nb_timed_peq.notify(*req, phase, delay);
  //  }
  // End the transaction
  nb_fw_transactor[payload.get_command()]->end_transaction(h, status);
  return status;
}

template <typename TYPES>
void recorder<TYPES>::b_transport(typename TYPES::tlm_payload_type& payload, sc_core::sc_time& t) {
  if (!enabled()) {
    fw_port->b_transport(payload, t);
    return;
  }
}

template <typename TYPES>
bool recorder<TYPES>::get_direct_mem_ptr(typename TYPES::tlm_payload_type&, tlm::tlm_dmi& dmi_data) {
  return false;
}

template <typename TYPES>
unsigned int recorder<TYPES>::transport_dbg(typename TYPES::tlm_payload_type&) {
  return 0;
}

template <typename TYPES>
void recorder<TYPES>::invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range) {}

template <typename TYPES>
bool recorder<TYPES>::enabled() const {
  return m_db != nullptr && tracing_enabled.value;
}

static constexpr std::string_view get_parent(char const* hier_name) {
  std::string_view ret(hier_name);
  auto pos = ret.rfind('.');
  if (pos == std::string::npos) {
    return ret;
  } else {
    return ret.substr(0, pos);
  }
}

template <typename TYPES>
recorder<TYPES>::recorder(const char* name, sc_port_b<tlm::tlm_fw_transport_if<TYPES>>& fw_port,
                          sc_port_b<tlm::tlm_bw_transport_if<TYPES>>& bw_port, bool recording_enabled, scv_tr_db* tr_db)
    : sc_core::sc_object(name),
      tracing_enabled("enableTracing", recording_enabled),
      timed_enabled("enableTimed", recording_enabled),
      fw_port(fw_port),
      bw_port(bw_port),
      m_db(tr_db),
      m_base_name{::get_parent(sc_core::sc_object::name())} {
  this->add_attribute(tracing_enabled);
  this->add_attribute(timed_enabled);
  if (tracing_enabled.value) {
    nb_fw_transactor.reserve(3);
    nb_bw_transactor.reserve(3);
    auto* nb_fw_stream = new scv_tr_stream((m_base_name + "_nb_fw").c_str(), "TRANSACTOR", m_db);
    nb_fw_transactor[tlm::TLM_READ_COMMAND] =
        new scv_tr_generator<std::string, tlm::tlm_sync_enum>("read", *nb_fw_stream, "tlm_phase", "tlm_sync");
    nb_fw_transactor[tlm::TLM_WRITE_COMMAND] =
        new scv_tr_generator<std::string, tlm::tlm_sync_enum>("write", *nb_fw_stream, "tlm_phase", "tlm_sync");
    nb_fw_transactor[tlm::TLM_IGNORE_COMMAND] =
        new scv_tr_generator<std::string, tlm::tlm_sync_enum>("ignore", *nb_fw_stream, "tlm_phase", "tlm_sync");

    auto* nb_bw_stream = new scv_tr_stream((m_base_name + "_nb_bw").c_str(), "TRANSACTOR", m_db);
    nb_bw_transactor[tlm::TLM_READ_COMMAND] =
        new scv_tr_generator<std::string, tlm::tlm_sync_enum>("read", *nb_bw_stream, "tlm_phase", "tlm_sync");
    nb_bw_transactor[tlm::TLM_WRITE_COMMAND] =
        new scv_tr_generator<std::string, tlm::tlm_sync_enum>("write", *nb_bw_stream, "tlm_phase", "tlm_sync");
    nb_bw_transactor[tlm::TLM_IGNORE_COMMAND] =
        new scv_tr_generator<std::string, tlm::tlm_sync_enum>("ignore", *nb_bw_stream, "tlm_phase", "tlm_sync");
  }
}

template <typename TYPES>
recorder<TYPES>::recorder(sc_port_b<tlm::tlm_fw_transport_if<TYPES>>& fw_port,
                          sc_port_b<tlm::tlm_bw_transport_if<TYPES>>& bw_port, bool recording_enabled, scv_tr_db* tr_db)
    : recorder{sc_core::sc_gen_unique_name("recorder"), fw_port, bw_port, recording_enabled, tr_db} {}
