#include "command_line.hpp"
#include "dummy_bus.hpp"
#include "ram_agent.hpp"
#include "testbench.hpp"
int sc_main(int argc, char* argv[]) {
  command_line{argc, argv};
  dummy_bus<64, 64> vif{sc_core::sc_module_name{"bus"}};

  uvm::uvm_config_db<ibus*>::set(nullptr, "*.ram_agent.*", "vif", &vif);

  uvm::run_test();
  return 0;
}
