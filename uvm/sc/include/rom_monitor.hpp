#ifndef ROM_MONITOR
#define ROM_MONITOR
#include <uvm>

#include "ibus.hpp"
#include "rom_sequence_item.hpp"

namespace uv {
class rom_monitor : public uvm::uvm_monitor {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_COMPONENT_UTILS(uv::rom_monitor)
  DISABLE_WARNING_POP

  rom_monitor(const std::string& name = "rom_monitor") : uvm::uvm_monitor{uvm::uvm_component_name{name.c_str()}} {}
  ~rom_monitor() = default;

 protected:
  virtual void build_phase(uvm::uvm_phase& phase) override {
    uvm::uvm_monitor::build_phase(phase);
    auto ok = uvm::uvm_config_db<ibus*>::get(this, "*", "vif", m_vif);
    if (!ok) {
      UVM_FATAL(get_name(), "Virtual interface not defined! Simulation aborted!");
    }
  }

  virtual void run_phase(uvm::uvm_phase& phase) override {
    rom_sequence_item inst;
    while (true) {
      UVM_INFO(get_name(), "Run phase", uvm::UVM_FULL);

      UVM_INFO(get_name(), "Monitor Run", uvm::UVM_FULL);
      sc_core::wait(1);
    }
  }

 private:
  ibus* m_vif;
};
}  // namespace uv
#endif  // ROM_MONITOR
