#ifndef TLM2AXI4_HPP
#define TLM2AXI4_HPP
#include <tlm_utils/simple_target_socket.h>

#include <systemc>
template <unsigned int ADDR_WIDTH, unsigned int DATA_WIDTH = 32>
class tlm2axi4 : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<tlm2axi4<DATA_WIDTH>> m_target;
  using address_type = sc_dt::sc_bv<ADDR_WIDTH>;
  using data_type = sc_dt::sc_bv<DATA_WIDTH>;
  sc_core::sc_in<sc_dt::sc_bv<ADDR_WIDTH>> m_waddr;
  sc_core::sc_out<sc_dt::sc_bv<DATA_WIDTH>> m_wdata;
  sc_core::sc_out<sc_dt::sc_bv<ADDR_WIDTH>> m_raddr;
  sc_core::sc_in<sc_dt::sc_bv<DATA_WIDTH>> m_rdata;
  sc_core::sc_in<bool> m_clk;
  sc_core::sc_in<bool> m_resetn;

  tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload&, tlm::tlm_phase&, sc_core::sc_time&) {
    std::cout << "bridge received" << std::endl;
    return tlm::TLM_UPDATED;
  }

  tlm2axi4(const sc_core::sc_module_name& name) : sc_module(name) {
    m_target.register_nb_transport_fw(this, &tlm2axi4<ADDR_WIDTH, DATA_WIDTH>::nb_transport_fw);
  }
};
#endif  // TLM2AXI4_HPP
