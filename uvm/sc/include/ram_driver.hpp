#ifndef RAM_DRIVER
#define RAM_DRIVER
#include <uvm>

#include "ibus.hpp"
#include "ram_sequence_item.hpp"

namespace uv {
class ram_driver : public uvm::uvm_driver<ram_sequence_item> {
 public:
  UVM_OBJECT_UTILS(uv::ram_driver)

  ram_driver(const std::string& name = "ram_driver")
      : uvm::uvm_driver<ram_sequence_item>{uvm::uvm_component_name{name.c_str()}}, m_item{nullptr} {}
  ~ram_driver() = default;

 protected:
  void build_phase(uvm::uvm_phase& phase) override {
    m_item = ram_sequence_item::type_id::create("ram_sequence_item", this);

    if (m_item == nullptr) {
      UVM_FATAL(get_name(), "Cannot create ram sequence item!");
    }

    auto ok = uvm::uvm_config_db<ibus*>::get(this, "*", "vif", m_vif);
    if (!ok) {
      UVM_FATAL(get_name(), "Virtual interface not defined!Simulation aborted!");
    }
  }

  [[noreturn]] void run_phase(uvm::uvm_phase& phase) override {
    UVM_INFO(get_name(), "Run phase", uvm::UVM_FULL);

    while (true) {
      seq_item_port->get_next_item(*m_item);
      UVM_INFO(get_name(), "Transfer", uvm::UVM_FULL);
      seq_item_port->item_done();
    }
  }

  ram_sequence_item* m_item{};
  ibus* m_vif;
};
}  // namespace uv
#endif  // RAM_DRIVER
