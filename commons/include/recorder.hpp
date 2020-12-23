#pragma once
#include <scv.h>
#include <tlm.h>

template <typename TYPES = tlm::tlm_base_protocol_types>
class recorder : public virtual tlm::tlm_fw_transport_if<TYPES>,
                 public virtual tlm::tlm_bw_transport_if<TYPES>,
                 public virtual sc_core::sc_object {
 public:
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
  //! \brief the port where fw accesses are forwarded to
  sc_core::sc_port_b<tlm::tlm_fw_transport_if<TYPES>>& fw_port;

  //! \brief the port where bw accesses are forwarded to
  sc_core::sc_port_b<tlm::tlm_bw_transport_if<TYPES>>& bw_port;

  //! \brief the attribute to selectively enable/disable recording
  sc_core::sc_attribute<bool> tracing_enabled;

  //! \brief the attribute to selectively enable/disable timed recording
  sc_core::sc_attribute<bool> timed_enabled;

  //! event queue to hold time points of non-blocking transactions
  //  tlm_utils::peq_with_cb_and_phase<recorder, recording_types> nb_timed_peq;

  scv_tr_db* m_db;
};

template <typename TYPES>
tlm::tlm_sync_enum recorder<TYPES>::nb_transport_bw(typename TYPES::tlm_payload_type& trans,
                                                    typename TYPES::tlm_phase_type& phase, sc_core::sc_time& t) {
  return tlm::TLM_UPDATED;
}

template <typename TYPES>
tlm::tlm_sync_enum recorder<TYPES>::nb_transport_fw(typename TYPES::tlm_payload_type&, typename TYPES::tlm_phase_type&,
                                                    sc_core::sc_time&) {
  return tlm::TLM_UPDATED;
}

template <typename TYPES>
void recorder<TYPES>::b_transport(typename TYPES::tlm_payload_type&, sc_core::sc_time& t) {}

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
  return false;
}
template <typename TYPES>
recorder<TYPES>::recorder(const char* name, sc_port_b<tlm::tlm_fw_transport_if<TYPES>>& fw_port,
                          sc_port_b<tlm::tlm_bw_transport_if<TYPES>>& bw_port, bool recording_enabled, scv_tr_db* tr_db)
    : sc_core::sc_object(name),
      tracing_enabled("enableTracing", recording_enabled),
      timed_enabled("enableTimed", recording_enabled),
      fw_port(fw_port),
      bw_port(bw_port),
      m_db(tr_db) {
  this->add_attribute(tracing_enabled);
  this->add_attribute(timed_enabled);
}

template <typename TYPES>
recorder<TYPES>::recorder(sc_port_b<tlm::tlm_fw_transport_if<TYPES>>& fw_port,
                          sc_port_b<tlm::tlm_bw_transport_if<TYPES>>& bw_port, bool recording_enabled, scv_tr_db* tr_db)
    : recorder{sc_core::sc_gen_unique_name("recorder"), fw_port, bw_port, recording_enabled, tr_db} {}
