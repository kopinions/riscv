#ifndef ROM_SEQUENCE_ITEM
#define ROM_SEQUENCE_ITEM
#include <uvm>
#include <warnings.hpp>

namespace uv {
class rom_sequence_item : public uvm::uvm_sequence_item {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_OBJECT_UTILS(uv::rom_sequence_item)
  DISABLE_WARNING_POP

  rom_sequence_item() {}

  explicit rom_sequence_item(const std::string& name) : uvm::uvm_sequence_item{name}, address{0}, inst{0} {}

  unsigned int address;
  unsigned int inst;

  void do_copy(const uvm::uvm_object& rhs) override {
    auto other = dynamic_cast<const rom_sequence_item*>(&rhs);
    if (other != nullptr) {
      *this = *other;
    } else {
      UVM_ERROR(get_name(), "Error in do_copy");
    }
  }
};
}  // namespace uv
#endif  // ROM_SEQUENCE_ITEM
