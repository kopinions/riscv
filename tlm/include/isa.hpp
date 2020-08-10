#ifndef ISA_HPP
#define ISA_HPP
#include "instruction.hpp"
using extension = enum { BASE, M, A, F };

template <unsigned int WIDTH>
constexpr std::uint8_t opcode(typename bits_helper<normalize(WIDTH)>::type inst) {
  return std::uint8_t(inst & 0x7F);
};

template <unsigned int WIDTH = 32, extension = BASE>
class isa {
 public:
  using opcode = enum {

  };
  using type = typename bits_helper<normalize(WIDTH)>::type;

  virtual const instruction<WIDTH>& from(type v) const { return rtype<WIDTH>(); }
};

template <unsigned int WIDTH>
class isa<WIDTH, M> : public sc_core::sc_module {
 public:
  using opcode = enum {

  };
  using type = typename bits_helper<normalize(WIDTH)>::type;

  virtual const instruction<WIDTH>& from(type v) const { return rtype<WIDTH>(); }
};

template <unsigned int WIDTH>
class isa<WIDTH, A> : public sc_core::sc_module {
 public:
  using opcode = enum {

  };
  using type = typename bits_helper<normalize(WIDTH)>::type;

  virtual const instruction<WIDTH>& from(type v) const { return rtype<WIDTH>(); }
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
