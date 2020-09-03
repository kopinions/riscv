#ifndef TESTBENCH_HPP
#define TESTBENCH_HPP
#include <uvm>

#include "ram_agent.hpp"
#include "rom_agent.hpp"
#include "scoreboard.hpp"

namespace uv {

class testbench : public uvm::uvm_env {
 public:
  friend class test;
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
      UVM_FATAL(get_name(), "Cannot create Ram agent! Simulation aborted!");
    }
    m_rom_agent = rom_agent::type_id::create("rom_agent", this);
    if (m_rom_agent == nullptr) {
      UVM_FATAL(get_name(), "Cannot create rom agent! Simulation aborted!");
    }

    m_sequencer = sequencer::type_id::create("sequencer", this);
    if (m_sequencer == nullptr) {
      UVM_FATAL(get_name(), "Cannot create sequencer! Simulation aborted!");
    }

    m_scoreboard = scoreboard::type_id::create("scoreboard", this);
    if (m_scoreboard == nullptr) {
      UVM_FATAL(get_name(), "Cannot create m_scoreboard! Simulation aborted!");
    }

    uvm::uvm_config_db<int>::set(this, "ram_agent", "is_active", uvm::UVM_ACTIVE);
    uvm::uvm_config_db<int>::set(this, "rom_agent", "is_active", uvm::UVM_ACTIVE);
  }

  virtual void connect_phase(uvm::uvm_phase& phase) override {
    uvm::uvm_env::connect_phase(phase);
    m_sequencer->m_ram_sequencer = m_ram_agent->m_ram_sequencer;
    m_sequencer->m_rom_sequencer = m_rom_agent->m_rom_sequencer;

    m_ram_agent->m_analysis_port.connect(m_scoreboard->m_ram_analysis_export);
    m_rom_agent->m_analysis_port.connect(m_scoreboard->m_rom_analysis_export);
  }

 private:
  ram_agent* m_ram_agent;
  rom_agent* m_rom_agent;
  scoreboard* m_scoreboard;
  sequencer* m_sequencer;
};

}  // namespace uv
#endif  // TESTBENCH_HPP
