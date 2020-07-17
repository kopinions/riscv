#include <command_line.hpp>
#include <dummy_bus.hpp>
#include <ibus.hpp>
#include <matrix_top.hpp>

int sc_main(int argc, char* argv[]) {
  command_line{argc, argv};
  dummy_bus<64, 64> vif{sc_core::sc_module_name{"code"}};
  dummy_bus<64, 64> data_vif{sc_core::sc_module_name{"data"}};

  matrix_system dut{"matrix_system"};
  dut.m_code->m_initiators.bind(vif.target_socket);
  dut.m_data->m_initiators.bind(data_vif.target_socket);

  uvm::uvm_config_db<ibus*>::set(nullptr, "*.ram_agent.*", "vif", &vif);

  uvm::run_test();
  return 0;
}
