#ifndef PACKET_HPP
#define PACKET_HPP
#include <uvm>
#include <warnings.hpp>
namespace uv {

class packet : public uvm::uvm_object {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_OBJECT_UTILS(uv::packet)
  DISABLE_WARNING_POP

  bool do_compare(const uvm_object &rhs, const uvm::uvm_comparer *comparer) const override {
    return uvm_object::do_compare(rhs, comparer);
  }

  packet() {}

  packet(const packet &) = default;
  packet(packet &&) = default;
  packet &operator=(const packet &) = default;
  packet &operator=(packet &&) = default;

  packet(const std::string &name) : uvm_object(name) {}
};
}  // namespace uv
#endif  // PACKET_HPP
