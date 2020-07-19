#ifndef TLM2AXI4_HPP
#define TLM2AXI4_HPP
#include <systemc>
template <unsigned int WIDTH = 32>
class tlm2axi4 : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<tlm2axi4<WIDTH>> m_target;
  using address_type = sc_dt::sc_bv<WIDTH>;
  using data_type = sc_dt::sc_bv<WIDTH>;
  mutable sc_core::sc_signal<address_type> s_addr{"addr"};
  sc_core::sc_signal<data_type> s_data{"data"};
  
  tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload&, tlm::tlm_phase&, sc_core::sc_time&) {
    return tlm::TLM_UPDATED;
  }

  tlm2axi4(const sc_core::sc_module_name& name) : sc_module(name) {
    m_target.register_nb_transport_fw(this, &tlm2axi4<WIDTH>::nb_transport_fw);
  }
};
#endif  // TLM2AXI4_HPP
