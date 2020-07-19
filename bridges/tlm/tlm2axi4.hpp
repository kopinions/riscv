#ifndef TLM2AXI4_HPP
#define TLM2AXI4_HPP
<<<<<<< HEAD
#include <systemc>
template <unsigned int WIDTH = 32>
class tlm2axi4 : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<tlm2axi4<WIDTH>> m_target;
  using address_type = sc_dt::sc_bv<WIDTH>;
  using data_type = sc_dt::sc_bv<WIDTH>;
  mutable sc_core::sc_signal<address_type> s_addr{"addr"};
  sc_core::sc_signal<data_type> s_data{"data"};
  
=======
#include <tlm_utils/simple_target_socket.h>

#include <systemc>
template <unsigned int ADDR_WIDTH, unsigned int DATA_WIDTH = 32>
class tlm2axi4 : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<tlm2axi4<DATA_WIDTH>> m_target;
  using address_type = sc_dt::sc_bv<ADDR_WIDTH>;
  using data_type = sc_dt::sc_bv<DATA_WIDTH>;
  sc_core::sc_in<sc_dt::sc_bv<ADDR_WIDTH>> m_waddr;
  sc_core::sc_out<sc_dt::sc_bv<ADDR_WIDTH>> m_raddr;
  sc_core::sc_out<sc_dt::sc_bv<DATA_WIDTH>> m_wdata;
  sc_core::sc_in<sc_dt::sc_bv<DATA_WIDTH>> m_rdata;
  sc_core::sc_in<bool> m_clk;
  sc_core::sc_in<bool> m_resetn;


>>>>>>> 1e06222... feat: should able to connect the bridge to the dut and uvm
  tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload&, tlm::tlm_phase&, sc_core::sc_time&) {
    return tlm::TLM_UPDATED;
  }

  tlm2axi4(const sc_core::sc_module_name& name) : sc_module(name) {
<<<<<<< HEAD
    m_target.register_nb_transport_fw(this, &tlm2axi4<WIDTH>::nb_transport_fw);
=======
    m_target.register_nb_transport_fw(this, &tlm2axi4<ADDR_WIDTH, DATA_WIDTH>::nb_transport_fw);
>>>>>>> 1e06222... feat: should able to connect the bridge to the dut and uvm
  }
};
#endif  // TLM2AXI4_HPP
