#ifndef RAM_SEQUENCER
#define RAM_SEQUENCER
#include <uvm>

#include "ram_sequence_item.hpp"

namespace uv {
class ram_sequencer : public uvm::uvm_sequencer<ram_sequence_item> {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_COMPONENT_UTILS(uv::ram_sequencer)
  DISABLE_WARNING_POP

  ram_sequencer(const std::string &name)
      : uvm::uvm_sequencer<ram_sequence_item>{uvm::uvm_component_name{name.c_str()}} {}

  ~ram_sequencer() = default;
};
}  // namespace uv
#endif  // RAM_SEQUENCER
