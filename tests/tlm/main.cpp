#include <command_line.hpp>
#include <dummy_bus.hpp>
#include <ibus.hpp>
#include <matrix_top.hpp>
#include <bridges.hpp>

int sc_main(int argc, char* argv[]) {
  command_line{argc, argv};
  dummy_bus<64, 64> vif{sc_core::sc_module_name{"code"}};
  dummy_bus<64, 64> data_vif{sc_core::sc_module_name{"data"}};

  tlm2axi4<32> t2a_code{"code_bridge"};
  tlm2axi4<32> t2a_data{"data_bridge"};

  matrix_system dut{"matrix_system"};
  dut.m_code->m_initiators.bind(t2a_code.m_target);
  dut.m_data->m_initiators.bind(t2a_data.m_target);

  uvm::uvm_config_db<ibus*>::set(nullptr, "*.ram_agent.*", "vif", &vif);

  uvm::run_test();
  return 0;
}
