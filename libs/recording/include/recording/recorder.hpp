#pragma once
#include <scv.h>
#include <tlm.h>

#include <array>

#include "recording/payload_memory_manager.hpp"
#include "recording/recordable_data.hpp"
#include "recording/recording_extension.hpp"

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

#include "recording/recorder.tpp"