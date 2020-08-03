#ifndef ROM_AGENT
#define ROM_AGENT
#include <uvm>

#include "instruction.hpp"
#include "rom_driver.hpp"
#include "rom_monitor.hpp"
#include "rom_sequencer.hpp"
#include "warnings.hpp"
namespace uv {
class rom_agent : public uvm::uvm_agent {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_COMPONENT_UTILS(uv::rom_agent)
  DISABLE_WARNING_POP

  explicit rom_agent(const std::string& name = "rom_agent")
      : uvm::uvm_agent{uvm::uvm_component_name{name.c_str()}},
        m_rom_sequencer{nullptr},
        m_rom_driver{nullptr},
        m_rom_monitor{nullptr} {}

  ~rom_agent() = default;

 protected:
  virtual void build_phase(uvm::uvm_phase& phase) override {
    uvm::uvm_agent::build_phase(phase);

    UVM_INFO(get_name(), "Build phase", uvm::UVM_FULL);

    if (get_is_active() == uvm::UVM_ACTIVE) {
      UVM_INFO(get_name(), "is set to UVM_ACTIVE", uvm::UVM_FULL);

      m_rom_sequencer = rom_sequencer::type_id::create("rom_sequencer", this);
      if (!m_rom_sequencer) {
        UVM_FATAL(get_name(), "Sequencer not defined! Simulation aborted!");
      }

      m_rom_driver = rom_driver::type_id::create("rom_driver", this);
      if (!m_rom_driver) {
        UVM_FATAL(get_name(), "Driver not defined! Simulation aborted!");
      }
    } else {
      UVM_INFO(get_name(), "is set to UVM_PASSIVE", uvm::UVM_FULL);
    }

    m_rom_monitor = rom_monitor::type_id::create("rom_monitor", this);
    if (!m_rom_monitor) {
      UVM_FATAL(get_name(), "Monitor not defined! Simulation aborted!");
    }
  }

  virtual void connect_phase(uvm::uvm_phase& phase) override {
    uvm::uvm_agent::connect_phase(phase);
    UVM_INFO(get_name(), "Connect phase", uvm::UVM_FULL);

    if (get_is_active() == uvm::UVM_ACTIVE) {
      m_rom_driver->seq_item_port.connect(m_rom_sequencer->seq_item_export);
    }
  }

  rom_sequencer* m_rom_sequencer;
  rom_driver* m_rom_driver;
  rom_monitor* m_rom_monitor;
};
}  // namespace uv
#endif  // ROM_AGENT
