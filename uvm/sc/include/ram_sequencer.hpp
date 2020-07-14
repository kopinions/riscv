#ifndef RAM_SEQUENCER
#define RAM_SEQUENCER
#include <uvm>

#include "ram_sequence_item.hpp"

namespace uv {
class ram_sequencer : public uvm::uvm_sequencer<ram_sequence_item> {
 public:
  UVM_OBJECT_UTILS(uv::ram_sequencer)

  ram_sequencer(const std::string &name)
      : uvm::uvm_sequencer<ram_sequence_item>{uvm::uvm_component_name{name.c_str()}} {}

  ~ram_sequencer() = default;
};
}  // namespace uv
#endif  // RAM_SEQUENCER
