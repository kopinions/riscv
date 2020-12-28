#pragma once
#include <scv.h>
#include <tlm.h>
#include <tlm_utils/peq_with_cb_and_phase.h>

#include <array>

#include "recording/payload_memory_manager.hpp"
#include "recording/recordable_data.hpp"
#include "recording/recording_extension.hpp"
#include "recording/relationship.hpp"
#include "recording/timed.hpp"
#include "recording/tlm_recordable_payload.hpp"

template <typename TYPES = tlm::tlm_base_protocol_types>
class recorder : public virtual tlm::tlm_fw_transport_if<TYPES>,
                 public virtual tlm::tlm_bw_transport_if<TYPES>,
                 public virtual sc_core::sc_object {
 public:
  using recordable_protocol_types = tlm_recordable_protocol_types<TYPES>;
  using mm = payload_memory_manager<recordable_protocol_types>;
  using recordable_payload_type = typename recordable_protocol_types::tlm_payload_type;
  using port_type = sc_core::sc_port_b<tlm::tlm_fw_transport_if<TYPES>>;
  using export_type = sc_core::sc_port_b<tlm::tlm_bw_transport_if<TYPES>>;

  recorder(port_type& fw_port, export_type& bw_port, bool recording_enabled = true,
           scv_tr_db* tr_db = scv_tr_db::get_default_db());

  recorder(const char* name, port_type& fw_port, export_type& bw_port, bool recording_enabled = true,
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
  port_type& fw_port;
  export_type& bw_port;
  sc_core::sc_attribute<bool> tracing_enabled;
  sc_core::sc_attribute<bool> timed_enabled;

  //! transaction generator handle for forward non-blocking transactions
  std::vector<scv_tr_generator<std::string, tlm::tlm_sync_enum>*> nb_fw_transactor;
  //! transaction generator handle for backward non-blocking transactions
  std::vector<scv_tr_generator<std::string, tlm::tlm_sync_enum>*> nb_bw_transactor;
  timed<TYPES> m_nb_timed;
  std::string m_base_name;
  scv_tr_db* m_db;
};

#include "recording/recorder.tpp"