#ifndef ROM_SEQUENCE
#define ROM_SEQUENCE
#include <uvm>
#include <vector>
#include <warnings.hpp>

#include "instruction.hpp"
#include "rom_sequencer.hpp"

namespace uv {
class rom_sequence : public uvm::uvm_sequence<instruction> {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_OBJECT_UTILS(uv::rom_sequence)
  DISABLE_WARNING_POP

  rom_sequence() : rom_sequence{"rom_sequence"} {}

  explicit rom_sequence(const std::string& name) : uvm::uvm_sequence<instruction>{name}, m_items{} {}

 protected:
  void pre_body() override {
    auto starting_phase = get_starting_phase();
    if (starting_phase != nullptr) {
      starting_phase->raise_objection(this);
    }

    m_sequencer = dynamic_cast<uv::rom_sequencer*>(get_sequencer());
  };

  void body() override {
    UVM_INFO(get_name(), "Starting sequence", uvm::UVM_FULL);
    instruction* item = instruction::type_id::create("sequence_item");
    uvm::uvm_sequence_item* rsp;
    start_item(item);
    finish_item(item);
    UVM_INFO(get_name(), "Finishing sequence", uvm::UVM_FULL);
  };

  void post_body() override {
    auto starting_phase = get_starting_phase();
    if (starting_phase != nullptr) {
      starting_phase->drop_objection(this);
    }
  };

  std::vector<instruction> m_items;
  uv::rom_sequencer* m_sequencer;
};

}  // namespace uv
#endif  // ROM_SEQUENCE
