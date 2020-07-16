#ifndef TESTBENCH_HPP
#define TESTBENCH_HPP
#include <uvm>

#include "ram_agent.hpp"

namespace uv {

class testbench : public uvm::uvm_env {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_COMPONENT_UTILS(uv::testbench)
  DISABLE_WARNING_POP

  testbench() : uvm::uvm_env{uvm::uvm_component_name{"testbench"}} {}

  testbench(const std::string& name = "testbench") : uvm::uvm_env{uvm::uvm_component_name{name.c_str()}} {}

 protected:
  virtual void build_phase(uvm::uvm_phase& phase) override {
    uvm::uvm_env::build_phase(phase);
    m_ram_agent = ram_agent::type_id::create("ram_agent", this);
    if (m_ram_agent == nullptr) {
      UVM_FATAL(get_name(),
                "Cannot create Ram agent!"
                " Simulation aborted!");
    }

    uvm::uvm_config_db<int>::set(this, "ram_agent", "is_active", uvm::UVM_ACTIVE);
  }

  virtual void connect_phase(uvm::uvm_phase& phase) override { uvm::uvm_env::connect_phase(phase); }

 private:
  ram_agent* m_ram_agent;
};

}  // namespace uv
#endif  // TESTBENCH_HPP
