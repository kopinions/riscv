#ifndef ROM_SEQUENCE
#define ROM_SEQUENCE
#include <uvm>
#include <vector>
#include <warnings.hpp>

#include "ibus.hpp"
#include "rom_sequence_item.hpp"
#include "rom_sequencer.hpp"

namespace uv {
class rom_sequence : public uvm::uvm_sequence<rom_sequence_item> {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_OBJECT_UTILS(uv::rom_sequence)
  DISABLE_WARNING_POP

  rom_sequence() : rom_sequence{"rom_sequence"} {}

  explicit rom_sequence(const std::string& name) : uvm::uvm_sequence<rom_sequence_item>{name}, m_items{} {}

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
    rom_sequence_item* req = rom_sequence_item::type_id::create("req");
    rom_sequence_item* rsp = rom_sequence_item::type_id::create("rsp");
    start_item(req);
    finish_item(req);
    get_response(req);
    UVM_INFO(get_name(), "req addr" + std::to_string(req->address), uvm::UVM_FULL);

    start_item(rsp);
    rsp->copy(*req);
    // TODO should able to generate the random instruction and send to the reference model
    // the reference model calculated and then compare with scoreboard
    rsp->inst = 0x007302B3;
    finish_item(rsp);

    UVM_INFO(get_name(), "Finishing sequence", uvm::UVM_FULL);
  };

  void post_body() override {
    auto starting_phase = get_starting_phase();
    if (starting_phase != nullptr) {
      starting_phase->drop_objection(this);
    }
  };

  std::vector<rom_sequence_item> m_items;
  uv::rom_sequencer* m_sequencer;
};

}  // namespace uv
#endif  // ROM_SEQUENCE
