#ifndef ROM_SEQUENCE
#define ROM_SEQUENCE
#include <uvm>
#include <vector>

#include "../../../include/commons/warnings.hpp"
#include "ibus.hpp"
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

  explicit rom_sequence(const std::string& name) : uvm::uvm_sequence<instruction>{name}, m_instructions{} {}

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
    std::uint64_t address;
    while(true) {
      instruction* req = instruction::type_id::create("req");
      start_item(req);
      finish_item(req);
      get_response(req);
      UVM_INFO(get_name(), "req addr" + std::to_string(req->address), uvm::UVM_FULL);
      address = req->address;
      instruction* rsp = instruction::type_id::create("rsp");
      start_item(rsp);
      rsp->copy(*req);
      // TODO should able to generate the random instruction and send to the reference model
      // the reference model calculated and then compare with scoreboard
      rsp->inst = m_instructions[req->address].inst;
      finish_item(rsp);
      UVM_INFO(get_name(), "instruction" + std::to_string(m_instructions[req->address].inst), uvm::UVM_FULL);
      if (m_instructions[address].inst == 0x00000000) {
        break;
      }
    }

    UVM_INFO(get_name(), "Finishing sequence", uvm::UVM_FULL);
  };

  void post_body() override {
    auto starting_phase = get_starting_phase();
    if (starting_phase != nullptr) {
      starting_phase->drop_objection(this);
    }
  };
 public:
  void append(instruction i) { m_instructions[i.address] = i; }

 public:
  std::map<std::uint64_t, instruction> m_instructions;
};

}  // namespace uv
#endif  // ROM_SEQUENCE
