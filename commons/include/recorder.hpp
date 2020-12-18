#pragma once
#include <tlm.h>

template <typename TYPES = tlm::tlm_base_protocol_types>
class recorder : public virtual tlm::tlm_fw_transport_if<TYPES>,
                 public virtual tlm::tlm_bw_transport_if<TYPES>,
                 public virtual sc_core::sc_object {
 public:
  tlm::tlm_sync_enum nb_transport_bw(typename TYPES::tlm_payload_type&, typename TYPES::tlm_phase_type&,
                                     sc_core::sc_time&) override;

  tlm::tlm_sync_enum nb_transport_fw(typename TYPES::tlm_payload_type&, typename TYPES::tlm_phase_type&,
                                     sc_core::sc_time&) override;

  void b_transport(typename TYPES::tlm_payload_type&, sc_core::sc_time& t) override;

  void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range) override;

  bool get_direct_mem_ptr(typename TYPES::tlm_payload_type&, tlm::tlm_dmi& dmi_data) override;

  unsigned int transport_dbg(typename TYPES::tlm_payload_type&) override;

  bool enabled() const;
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
