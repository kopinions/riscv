#include "command_line.hpp"
#include "testbench.hpp"
#include "ram_agent.hpp" 
  
int sc_main(int argc, char*argv[]) {
  command_line{argc, argv};
  uvm::run_test();
  return 0;
}
