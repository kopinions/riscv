#ifndef RAM_SEQUENCE
#define RAM_SEQUENCE
#include <uvm>
#include <vector>
#include <warnings.hpp>

#include "ram_sequence_item.hpp"
#include "ram_sequencer.hpp"

namespace uv {
class ram_sequence : public uvm::uvm_sequence<ram_sequence_item> {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_OBJECT_UTILS(uv::ram_sequence)
  DISABLE_WARNING_POP

  ram_sequence() : ram_sequence{"ram_sequence"} {}

  explicit ram_sequence(const std::string& name) : uvm::uvm_sequence<ram_sequence_item>{name}, m_items{} {}
  void append(ram_sequence_item item){

  };

 protected:
  void pre_body() override {
    auto starting_phase = get_starting_phase();
    if (starting_phase != nullptr) {
      starting_phase->raise_objection(this);
    }

    m_ram_sequencer = dynamic_cast<uv::ram_sequencer*>(get_sequencer());
  };

  void body() override {
    UVM_INFO(get_name(), "Starting sequence", uvm::UVM_FULL);
    ram_sequence_item* item = ram_sequence_item::type_id::create("ram_sequence_item");
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

  std::vector<ram_sequence_item> m_items;
  uv::ram_sequencer* m_ram_sequencer;
};

}  // namespace uv
#endif  // RAM_SEQUENCE
