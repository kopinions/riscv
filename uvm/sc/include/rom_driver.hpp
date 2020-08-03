#ifndef ROM_DRIVER
#define ROM_DRIVER
#include <uvm>
#include <warnings.hpp>

#include "ibus.hpp"
#include "rom_sequence_item.hpp"

namespace uv {
class rom_driver : public uvm::uvm_driver<rom_sequence_item> {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_COMPONENT_UTILS(uv::rom_driver)
  DISABLE_WARNING_POP

  rom_driver(const std::string& name = "rom_driver")
      : uvm::uvm_driver<rom_sequence_item>{uvm::uvm_component_name{name.c_str()}},
        m_rom_sequence_item{nullptr},
        m_rom_sequence_item_rsp{nullptr} {}
  ~rom_driver() = default;

 protected:
  void build_phase(uvm::uvm_phase& phase) override {
    m_rom_sequence_item = rom_sequence_item::type_id::create("rom_sequence_item", this);

    if (m_rom_sequence_item == nullptr) {
      UVM_FATAL(get_name(), "Cannot create rom sequence item!");
    }
    m_rom_sequence_item_rsp = rom_sequence_item::type_id::create("rom_sequence_item_rsp", this);

    if (m_rom_sequence_item_rsp == nullptr) {
      UVM_FATAL(get_name(), "Cannot create rom sequence item rsp!");
    }

    auto ok = uvm::uvm_config_db<ibus*>::get(this, "*", "vif", m_vif);
    if (!ok) {
      UVM_FATAL(get_name(), "Virtual interface not defined!Simulation aborted!");
    }
  }

  [[noreturn]] void run_phase(uvm::uvm_phase& phase) override {
    UVM_INFO(get_name(), "Run phase", uvm::UVM_FULL);

    while (true) {
      seq_item_port->get_next_item(*m_rom_sequence_item);
      UVM_INFO(get_name(), "req", uvm::UVM_FULL);
      m_rom_sequence_item->address = 2;
      UVM_INFO(get_name(), "addr" + std::to_string(m_rom_sequence_item->address) , uvm::UVM_FULL);
      seq_item_port->item_done(*m_rom_sequence_item);

      seq_item_port->get_next_item(*m_rom_sequence_item_rsp);
      UVM_INFO(get_name(), "rsp", uvm::UVM_FULL);
      seq_item_port->item_done();
    }
  }

  rom_sequence_item* m_rom_sequence_item{};
  rom_sequence_item* m_rom_sequence_item_rsp{};
  ibus* m_vif;
};
}  // namespace uv
#endif  // ROM_DRIVER
