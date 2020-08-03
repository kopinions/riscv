#ifndef ROM_SEQUENCE_ITEM
#define ROM_SEQUENCE_ITEM
#include <uvm>
#include <warnings.hpp>

namespace uv {
class instruction : public uvm::uvm_sequence_item {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_OBJECT_UTILS(uv::instruction)
  DISABLE_WARNING_POP

  instruction() {}

  explicit instruction(const std::string& name) : uvm::uvm_sequence_item{name} {}
};
}  // namespace uv
#endif  // ROM_SEQUENCE_ITEM
