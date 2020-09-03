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

  explicit instruction(const std::string& name) : uvm::uvm_sequence_item{name}, address{0}, inst{0} {}

  instruction(std::uint64_t addr, unsigned int inst) : address{addr}, inst{inst} {};

  std::uint64_t address;
  unsigned int inst;

  void do_copy(const uvm::uvm_object& rhs) override {
    auto other = dynamic_cast<const instruction*>(&rhs);
    if (other != nullptr) {
      *this = *other;
    } else {
      UVM_ERROR(get_name(), "Error in do_copy");
    }
  }
};
}  // namespace uv
#endif  // ROM_SEQUENCE_ITEM
