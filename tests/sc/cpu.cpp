#include "command_line.hpp"
#include "testbench.hpp"
#include "ram_agent.hpp" 
  
int sc_main(int argc, char*argv[]) {
  command_line{argc, argv};
  uv::testbench<uv::ram_agent, uv::ram_agent> tb {new uv::ram_agent{}, new uv::ram_agent{}};
  return 0;
}
