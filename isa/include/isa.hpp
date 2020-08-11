#ifndef ISA_HPP
#define ISA_HPP

template <unsigned int WIDTH>
constexpr std::uint8_t opcode(typename bits_helper<normalize(WIDTH)>::type inst) {
  return std::uint8_t(inst & 0x7F);
};

template <unsigned int WIDTH>
constexpr unsigned int rs1(normalized<WIDTH> inst) {
  return (unsigned int)((inst & 0x000F8000) >> 15);
};

template <unsigned int WIDTH>
constexpr unsigned int rs2(normalized<WIDTH> inst) {
  return (unsigned int)((inst & 0x01F00000) >> 20);
};

template <unsigned int WIDTH>
constexpr unsigned int rd(normalized<WIDTH> inst) {
  return std::uint8_t((inst & 0xF80) >> 7);
};

template <unsigned int WIDTH>
constexpr normalized<WIDTH> iimm(normalized<WIDTH> inst) {
  // sign = (inst & 0x80000000) >> 31
  // mask = ~(sign-1) & 0xFFFFF000;
  // imm = imm[12:0] | mask
  return normalized<WIDTH>(((inst & 0xFFF00000) >> 20) | ((~(((inst & 0x80000000) >> 31) - 1)) & 0xFFFFF000));
};

template <unsigned int WIDTH>
constexpr normalized<WIDTH> simm(normalized<WIDTH> inst) {
  return normalize((inst & 0xF80) >> 7) | normalize(((inst & 0xFF000000) >> 24) << 5);
};

template <unsigned int WIDTH>
constexpr normalized<WIDTH> bimm(normalized<WIDTH> inst) {
  return normalized<WIDTH>(inst & 0x7F);
};

template <unsigned int WIDTH>
constexpr normalized<WIDTH> uimm(normalized<WIDTH> inst) {
  return normalized<WIDTH>(inst & 0x7F);
};

template <unsigned int WIDTH>
class decoded;

template <unsigned int WIDTH>
const normalized<WIDTH> value_of_register(unsigned int index);

struct reg {
  int operator()(int x) { return x / 3; }
};

template <unsigned int WIDTH = 32>
using reg_if = std::function<normalized<WIDTH>(unsigned int)>;

class isa {
 public:
  using type = enum { RTYPE, ITYPE, STYPE, BTYPE, UTYPE, JTYPE };
  using extension = enum { BASE, M, A, F };

  template <unsigned int WIDTH = 32>
  static type type_of(decoded<WIDTH> decoded) {
    switch (decoded.opcode) {
      case 0x33:
        return RTYPE;
      case 0x13:
        return ITYPE;
      default:
        return RTYPE;
    }
  };

  template <unsigned int WIDTH = 32>
  const decoded<WIDTH> decode(normalized<WIDTH> v, reg_if<WIDTH> accessor) const {
    uint8_t opcode = ::opcode<WIDTH>(v);
    unsigned int rs1_id = ::rs1<WIDTH>(v);
    normalized<WIDTH> rs1_value = accessor(rs1_id);
    unsigned int rs2_id = ::rs2<WIDTH>(v);
    normalized<WIDTH> rs2_value = accessor(rs2_id);
    switch (opcode) {
      case 0x33:
        return decoded<WIDTH>{opcode, rs1_value, rs2_value, ::rd<WIDTH>(v), normalized<WIDTH>{0}};
      case 0x13:
        return decoded<WIDTH>{opcode, rs1_value, rs2_value, ::rd<WIDTH>(v), ::iimm<WIDTH>(v)};
      default:
        return decoded<WIDTH>{0, normalized<WIDTH>{0}, normalized<WIDTH>{0}, 0, normalized<WIDTH>{0}};
    }
  }
};

template <unsigned int WIDTH = 32>
class decoded {
 public:
  decoded(std::uint8_t opcode, normalized<WIDTH> rs1, normalized<WIDTH> rs2, unsigned int rd, normalized<WIDTH> imm)
      : opcode(opcode), rs1{rs1}, rs2{rs2}, rd{rd}, imm{imm} {}

  decoded() : opcode{0} {}
  std::uint8_t opcode;
  isa::extension ext;
  normalized<WIDTH> rs1;
  normalized<WIDTH> rs2;
  unsigned int rd;
  normalized<WIDTH> imm;
  bool mem_access;
  normalized<WIDTH> mem_address;
  bool branch_valid;
  normalized<WIDTH> branch_target_address;
  bool inc_pc;

  decoded(const decoded&) = default;
};
#endif  // ISA_HPP
