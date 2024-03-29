#ifndef RAM_AGENT
#define RAM_AGENT
#include <uvm>

#include "../../../include/commons/warnings.hpp"
#include "packet.hpp"
#include "ram_driver.hpp"
#include "ram_monitor.hpp"
#include "ram_sequence_item.hpp"
#include "ram_sequencer.hpp"

namespace uv {
class ram_agent : public uvm::uvm_agent {
 public:
  friend class testbench;
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_COMPONENT_UTILS(uv::ram_agent)
  DISABLE_WARNING_POP

  explicit ram_agent(const std::string& name = "ram_agent")
      : uvm::uvm_agent{uvm::uvm_component_name{name.c_str()}},
        m_ram_sequencer{nullptr},
        m_ram_driver{nullptr},
        m_ram_monitor{nullptr} {}

  ~ram_agent() = default;

 protected:
  void build_phase(uvm::uvm_phase& phase) override {
    uvm::uvm_agent::build_phase(phase);

    UVM_INFO(get_name(), "Build phase", uvm::UVM_FULL);

    if (get_is_active() == uvm::UVM_ACTIVE) {
      UVM_INFO(get_name(), "is set to UVM_ACTIVE", uvm::UVM_FULL);

      m_ram_sequencer = ram_sequencer::type_id::create("ram_sequencer", this);
      if (!m_ram_sequencer) {
        UVM_FATAL(get_name(),
                  "Sequencer not defined!"
                  " Simulation aborted!");
      }


      m_ram_driver = ram_driver::type_id::create("ram_driver", this);
      if (!m_ram_driver) {
        UVM_FATAL(get_name(),
                  "Driver not defined!"
                  " Simulation aborted!");
      }
    } else {
      UVM_INFO(get_name(), "is set to UVM_PASSIVE", uvm::UVM_FULL);
    }

    m_ram_monitor = ram_monitor::type_id::create("ram_monitor", this);
    if (!m_ram_monitor) {
      UVM_FATAL(get_name(), "Monitor not defined! Simulation aborted!");
    }
  }

  virtual void connect_phase(uvm::uvm_phase& phase) override {
    uvm::uvm_agent::connect_phase(phase);
    UVM_INFO(get_name(), "Connect phase", uvm::UVM_FULL);

    if (get_is_active() == uvm::UVM_ACTIVE) {
      m_ram_driver->seq_item_port.connect(m_ram_sequencer->seq_item_export);
    }
  }

  ram_sequencer* m_ram_sequencer;
  ram_driver* m_ram_driver;
  ram_monitor* m_ram_monitor;
  uvm::uvm_analysis_port<packet> m_analysis_port;
};
}  // namespace uv
#endif  // RAM_AGENT
