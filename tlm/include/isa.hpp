#ifndef ISA_HPP
#define ISA_HPP
#include "instruction.hpp"
using extension = enum { BASE, M, A, F };

template <unsigned int WIDTH>
constexpr std::uint8_t opcode(typename bits_helper<normalize(WIDTH)>::type inst) {
  return std::uint8_t(inst & 0x7F);
};

template <unsigned int WIDTH = 32>
class inst {
 public:
  using type = typename bits_helper<normalize(WIDTH)>::type;
  inst(std::uint8_t opcode) : opcode(opcode) {}
  inst() : opcode{0} {}
  std::uint8_t opcode;
  extension ext;
  type rs1;
  type rs2;
  std::uint32_t rd;
  type imm;
  bool mem_access;
  type mem_address;
  bool branch_valid;
  type branch_target_address;
  bool inc_pc;
};

template <unsigned int WIDTH = 32, extension = BASE>
class isa {
 public:
  using opcode = enum {

  };
  using type = typename bits_helper<normalize(WIDTH)>::type;

  virtual const inst<WIDTH>& from(type v) const { return inst<WIDTH>{::opcode<WIDTH>(v)}; }
};

template <unsigned int WIDTH>
class isa<WIDTH, M> : public sc_core::sc_module {
 public:
  using opcode = enum {

  };
  using type = typename bits_helper<normalize(WIDTH)>::type;

  virtual const inst<WIDTH>& from(type v) const { return inst<WIDTH>(); }
};

template <unsigned int WIDTH>
class isa<WIDTH, A> : public sc_core::sc_module {
 public:
  using opcode = enum {

  };
  using type = typename bits_helper<normalize(WIDTH)>::type;

  virtual const inst<WIDTH>& from(type v) const { return inst<WIDTH>(); }
};

template <unsigned int WIDTH>
const isa<>& isa_router(typename bits_helper<normalize(WIDTH)>::type inst) {
  if (opcode<WIDTH>(inst) == 0b0110011) {
    const auto& isxx = isa<WIDTH, BASE>();
    return isxx;
  }
  throw 1;
}

#endif  // ISA_HPP
