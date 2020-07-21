#include <bridges.hpp>
#include <command_line.hpp>
#include <dummy_bus.hpp>
#include <ibus.hpp>
#include <matrix_top.hpp>
#include <bridges.hpp>

int sc_main(int argc, char* argv[]) {
  command_line{argc, argv};
  sc_core::sc_clock clk{"clk", sc_core::sc_time(1, sc_core::SC_US)};
  sc_core::sc_signal<bool> rst_n;

  dummy_bus<32, 32> code_vif{sc_core::sc_module_name{"code_vif"}};
  code_vif.m_clk(clk);
  code_vif.m_resetn(rst_n);
  dummy_bus<32, 32> data_vif{sc_core::sc_module_name{"data_vif"}};
  data_vif.m_clk(clk);
  data_vif.m_resetn(rst_n);

  tlm2axi4<32> t2a_code{"code_bridge"};
  tlm2axi4<32> t2a_data{"data_bridge"};

  t2a_code.m_clk(clk);
  t2a_code.m_resetn(rst_n);
  t2a_code.m_rdata(code_vif.m_rdata);
  t2a_code.m_wdata(code_vif.m_wdata);
  t2a_code.m_raddr(code_vif.m_raddr);
  t2a_code.m_waddr(code_vif.m_waddr);

  t2a_data.m_clk(clk);
  t2a_data.m_resetn(rst_n);
  t2a_data.m_rdata(data_vif.m_rdata);
  t2a_data.m_wdata(data_vif.m_wdata);
  t2a_data.m_raddr(data_vif.m_raddr);
  t2a_data.m_waddr(data_vif.m_waddr);

  tlm2axi4<32> t2a_code{"code_bridge"};
  tlm2axi4<32> t2a_data{"data_bridge"};

  matrix_system dut{"matrix_system"};
  dut.m_code->m_initiators.bind(t2a_code.m_target);
  dut.m_data->m_initiators.bind(t2a_data.m_target);
  sc_core::sc_start();

  uvm::uvm_config_db<ibus*>::set(nullptr, "*.ram_agent.*", "vif", &code_vif);

  uvm::run_test();
  return 0;
}
