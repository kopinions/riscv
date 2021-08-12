#ifndef ROM_DRIVER
#define ROM_DRIVER
#include <uvm>

#include "../../../include/commons/warnings.hpp"
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
      : uvm::uvm_driver<instruction>{uvm::uvm_component_name{name.c_str()}},
        m_rom_sequence_item{nullptr},
        m_rom_sequence_item_rsp{nullptr} {}
  ~rom_driver() = default;

 protected:
  void build_phase(uvm::uvm_phase& phase) override {
    m_rom_sequence_item = instruction::type_id::create("rom_sequence_item", this);

    if (m_rom_sequence_item == nullptr) {
      UVM_FATAL(get_name(), "Cannot create rom sequence item!");
    }
    m_rom_sequence_item_rsp = instruction::type_id::create("rom_sequence_item_rsp", this);

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
      if (m_vif->is_reset()) {
        m_vif->wait_for_reset_release();
      }
      m_vif->wait_abort_on_reset(m_vif->m_arvalid);
      m_vif->set_arready(true);
      const bitstream& araddr = m_vif->get_araddr();

      seq_item_port->get_next_item(*m_rom_sequence_item);
      UVM_INFO(get_name(), "req", uvm::UVM_FULL);
      m_rom_sequence_item->address = std::uint64_t(araddr);
      UVM_INFO(get_name(), "addr" + std::to_string(m_rom_sequence_item->address) , uvm::UVM_FULL);
      seq_item_port->item_done(*m_rom_sequence_item, true);

      seq_item_port->get_next_item(*m_rom_sequence_item_rsp);
      UVM_INFO(get_name(), "rsp" + std::to_string(m_rom_sequence_item_rsp->inst), uvm::UVM_FULL);

      m_vif->m_rvalid.write(true);
      bitstream d(32);
      d = m_rom_sequence_item_rsp->inst;
      m_vif->set_rdata(d);
      m_vif->wait_abort_on_reset(m_vif->m_rready);
      m_vif->m_rvalid.write(false);
      seq_item_port->item_done();
    }
  }

  instruction* m_rom_sequence_item{};
  instruction* m_rom_sequence_item_rsp{};
  ibus* m_vif;
};
}  // namespace uv
#endif  // ROM_DRIVER
