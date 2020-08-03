#ifndef ROM_DRIVER
#define ROM_DRIVER
#include <uvm>
#include <warnings.hpp>

#include "ibus.hpp"
#include "instruction.hpp"

namespace uv {
class rom_driver : public uvm::uvm_driver<instruction> {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_COMPONENT_UTILS(uv::rom_driver)
  DISABLE_WARNING_POP

  rom_driver(const std::string& name = "rom_driver")
      : uvm::uvm_driver<instruction>{uvm::uvm_component_name{name.c_str()}}, m_instruction{nullptr} {}
  ~rom_driver() = default;

 protected:
  void build_phase(uvm::uvm_phase& phase) override {
    m_instruction = instruction::type_id::create("instruction", this);

    if (m_instruction == nullptr) {
      UVM_FATAL(get_name(), "Cannot create rom sequence item!");
    }

    auto ok = uvm::uvm_config_db<ibus*>::get(this, "*", "vif", m_vif);
    if (!ok) {
      UVM_FATAL(get_name(), "Virtual interface not defined!Simulation aborted!");
    }
  }

  [[noreturn]] void run_phase(uvm::uvm_phase& phase) override {
    UVM_INFO(get_name(), "Run phase", uvm::UVM_FULL);

    while (true) {
      seq_item_port->get_next_item(*m_instruction);
      UVM_INFO(get_name(), "Transfer", uvm::UVM_FULL);
      seq_item_port->item_done();
    }
  }

  instruction* m_instruction{};
  ibus* m_vif;
};
}  // namespace uv
#endif  // ROM_DRIVER
