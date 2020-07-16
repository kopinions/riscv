#ifndef RAM_SEQUENCE_ITEM
#define RAM_SEQUENCE_ITEM
#include <uvm>

namespace uv {
class ram_sequence_item : public uvm::uvm_sequence_item {
 public:
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winconsistent-missing-override"
  UVM_OBJECT_UTILS(uv::ram_sequence_item)
#pragma GCC diagnostic pop

  ram_sequence_item() {}

  explicit ram_sequence_item(const std::string& name) : uvm::uvm_sequence_item{name} {}
};
}  // namespace uv
#endif  // RAM_SEQUENCE_ITEM
