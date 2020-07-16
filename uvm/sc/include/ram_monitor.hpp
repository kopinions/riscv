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
  virtual void build_phase(uvm::uvm_phase& phase) override {}

  virtual void run_phase(uvm::uvm_phase& phase) override {
    UVM_INFO(get_name(), "Run phase", uvm::UVM_FULL);

    UVM_INFO(get_name(), "Monitor Run", uvm::UVM_FULL);
    sleep(1);
  }
  std::string a;
};
}  // namespace uv
#endif  // RAM_MONITOR
