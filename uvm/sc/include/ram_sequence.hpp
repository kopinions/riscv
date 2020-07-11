#ifndef RAM_SEQUENCE
#define RAM_SEQUENCE
#include <uvm>

#include "ram_sequence_item.hpp"

namespace uv {
class ram_sequence : public uvm::uvm_sequence<ram_sequence_item> {
 public:
  UVM_OBJECT_UTILS(uv::ram_sequence)

  ram_sequence() : ram_sequence{"ram_sequence"} {}

  explicit ram_sequence(const std::string& name)
      : uvm::uvm_sequence<ram_sequence_item>{name}, m_items{} {}

 protected:
  void pre_body() override {
    if (starting_phase != nullptr) {
      starting_phase->raise_objection(this);
    }
  };

  void body() override {
    UVM_INFO(get_name(), "Starting sequence", uvm::UVM_FULL);
    for (auto& item : m_items) {
      start_item(&item);
      finish_item(&item);
    }
    UVM_INFO(get_name(), "Finishing sequence", uvm::UVM_FULL);
  };

  void post_body() override {
    if (starting_phase != nullptr) {
      starting_phase->drop_objection(this);
    }
  };

  std::vector<tx_sequence_item> m_items;
};

}  // namespace uv
#endif  // RAM_SEQUENCE
