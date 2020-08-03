#ifndef RAM_MONITOR
#define RAM_MONITOR
#include <uvm>

#include "ram_sequence_item.hpp"

namespace uv {
class ram_monitor : public uvm::uvm_monitor {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_COMPONENT_UTILS(uv::ram_monitor)
  DISABLE_WARNING_POP

  ram_monitor(const std::string& name = "ram_monitor") : uvm::uvm_monitor{uvm::uvm_component_name{name.c_str()}} {}
  ~ram_monitor() = default;

 protected:
  virtual void build_phase(uvm::uvm_phase& phase) override {
    uvm::uvm_monitor::build_phase(phase);
    auto ok = uvm::uvm_config_db<ibus*>::get(this, "*", "vif", m_vif);
    if (!ok) {
      UVM_FATAL(get_name(), "Virtual interface not defined!Simulation aborted!");
    }
  }

  virtual void run_phase(uvm::uvm_phase& phase) override {
    ram_sequence_item item;
    while (true) {
      UVM_INFO(get_name(), "Monitor Run", uvm::UVM_FULL);
      bitstream a(32);
      a[0] = 0;

      m_vif->read(a, 0);
      sc_core::wait(1);
    }
  }

 private:
  ibus* m_vif;
};
}  // namespace uv
#endif  // RAM_MONITOR
