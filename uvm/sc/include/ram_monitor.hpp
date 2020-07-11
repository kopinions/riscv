#ifndef RAM_MONITOR
#define RAM_MONITOR
#include <uvm>

#include "ram_sequence_item.hpp"

namespace uv {
class ram_monitor : public uvm::uvm_monitor {
 public:
  UVM_OBJECT_UTILS(uv::ram_monitor)

  explicit ram_monitor(const uvm::uvm_component_name& name)
      : uvm::uvm_monitor{name} {}

  ram_monitor(const std::string& name = "ram_monitor")
      : uvm::uvm_monitor{uvm::uvm_component_name{name.c_str()}} {}
  ~ram_monitor() = default;

 protected:
  void build_phase(uvm::uvm_phase& phase) override {}

  [[noreturn]] void run_phase(uvm::uvm_phase& phase) override {
    UVM_INFO(get_name(), "Run phase", uvm::UVM_FULL);

    while (true) {
      UVM_INFO(get_name(), "Monitor Run", uvm::UVM_FULL);
      sleep(1);
    }
  }
  std::string a;
};
}  // namespace uv
#endif  // RAM_MONITOR
