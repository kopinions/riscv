#pragma once

#include <functional>
#include <sstream>
#include <tlm>

template <typename RECORDABLE, typename TYPES = tlm::tlm_base_protocol_types>
class outbound : public RECORDABLE {
 public:
  using transaction_type = typename TYPES::tlm_payload_type;
  using phase_type = typename TYPES::tlm_phase_type;
  using sync_enum_type = tlm::tlm_sync_enum;
  using fw_interface_type = tlm::tlm_fw_transport_if<TYPES>;
  using bw_interface_type = tlm::tlm_bw_transport_if<TYPES>;

 public:
  outbound() : outbound(sc_core::sc_gen_unique_name("initiator_mixin_socket")) {}

  explicit outbound(const sc_core::sc_module_name& name) : RECORDABLE(name), bw_if(this->name()) {
    this->m_export.bind(bw_if);
  }

  void register_nb_transport_bw(std::function<sync_enum_type(transaction_type&, phase_type&, sc_core::sc_time&)> cb) {
    bw_if.set_transport_function(cb);
  }

  void register_invalidate_direct_mem_ptr(std::function<void(sc_dt::uint64, sc_dt::uint64)> cb) {
    bw_if.set_invalidate_direct_mem_function(cb);
  }

 private:
  class bw_transport_if : public tlm::tlm_bw_transport_if<TYPES> {
   public:
    using transport_fct = std::function<sync_enum_type(transaction_type&, phase_type&, sc_core::sc_time&)>;
    using invalidate_dmi_fct = std::function<void(sc_dt::uint64, sc_dt::uint64)>;

    explicit bw_transport_if(const std::string& name)
        : m_name(name), m_transport_ptr(0), m_invalidate_direct_mem_ptr(0) {}

    void set_transport_function(transport_fct p) {
      if (m_transport_ptr) {
        std::stringstream s;
        s << m_name << ": non-blocking callback allready registered";
        SC_REPORT_WARNING("/OSCI_TLM-2/simple_socket", s.str().c_str());
      } else {
        m_transport_ptr = p;
      }
    }

    void set_invalidate_direct_mem_function(invalidate_dmi_fct p) {
      if (m_invalidate_direct_mem_ptr) {
        std::stringstream s;
        s << m_name << ": invalidate DMI callback allready registered";
        SC_REPORT_WARNING("/OSCI_TLM-2/simple_socket", s.str().c_str());
      } else {
        m_invalidate_direct_mem_ptr = p;
      }
    }

    sync_enum_type nb_transport_bw(transaction_type& trans, phase_type& phase, sc_core::sc_time& t) {
      if (m_transport_ptr) return m_transport_ptr(trans, phase, t);
      std::stringstream s;
      s << m_name << ": no transport callback registered";
      SC_REPORT_ERROR("/OSCI_TLM-2/initiator_mixin", s.str().c_str());
      return tlm::TLM_ACCEPTED;  ///< unreachable code
    }

    void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range) {
      if (m_invalidate_direct_mem_ptr)  // forward call
        m_invalidate_direct_mem_ptr(start_range, end_range);
    }

   private:
    const std::string m_name;
    transport_fct m_transport_ptr;
    invalidate_dmi_fct m_invalidate_direct_mem_ptr;
  };

 private:
  bw_transport_if bw_if;
};