#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP

namespace uv {
class sequencer : public uvm::uvm_sequencer<> {
 public:
  friend class sequence;
  friend class testbench;
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_COMPONENT_UTILS(uv::sequencer)
  DISABLE_WARNING_POP

  sequencer() : uvm::uvm_sequencer<>{"sequencer"} {};

  explicit sequencer(const uvm::uvm_component_name& name)
      : uvm::uvm_sequencer<>{name}, m_ram_sequencer{nullptr}, m_rom_sequencer{nullptr} {};

  sequencer(sequencer&&) = delete;

  sequencer(const sequencer&) = delete;

  sequencer& operator=(sequencer&&) = delete;

  sequencer& operator=(const sequencer&) = delete;

  ~sequencer() override = default;

 private:
  ram_sequencer* m_ram_sequencer;
  rom_sequencer* m_rom_sequencer;
};
}  // namespace uv

#endif  // SEQUENCER_HPP
