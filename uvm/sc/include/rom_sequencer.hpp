#ifndef ROM_SEQUENCER
#define ROM_SEQUENCER
#include <uvm>

#include "instruction.hpp"

namespace uv {
class rom_sequencer : public uvm::uvm_sequencer<instruction> {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_COMPONENT_UTILS(uv::rom_sequencer)
  DISABLE_WARNING_POP

  rom_sequencer(const std::string &name)
      : uvm::uvm_sequencer<instruction>{uvm::uvm_component_name{name.c_str()}} {}

  ~rom_sequencer() = default;
};
}  // namespace uv
#endif  // ROM_SEQUENCER
