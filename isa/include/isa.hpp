#ifndef ISA_HPP
#define ISA_HPP
template <unsigned int WIDTH>
constexpr unsigned int rs1(normalized<WIDTH> inst) {
  return (unsigned int)((inst & 0x000F8000) >> 15);
}

template <unsigned int WIDTH>
constexpr unsigned int rs2(normalized<WIDTH> inst) {
  return (unsigned int)((inst & 0x01F00000) >> 20);
}

template <unsigned int WIDTH>
constexpr unsigned int rd(normalized<WIDTH> inst) {
  return std::uint8_t((inst & 0xF80) >> 7);
}

template <unsigned int WIDTH>
constexpr std::uint8_t func3(normalized<WIDTH> inst) {
  return std::uint8_t((inst & 0x700) >> 12);
}

template <unsigned int WIDTH>
constexpr std::uint8_t func7(normalized<WIDTH> inst) {
  return std::uint8_t((inst & 0xFE000000) >> 25);
}

template <unsigned int WIDTH>
constexpr normalized<WIDTH> iimm(normalized<WIDTH> inst) {
  // sign = (inst & 0x80000000) >> 31
  // mask = ~(sign-1) & 0xFFFFF000;
  // imm = imm[12:0] | mask
  return normalized<WIDTH>(((inst & 0xFFF00000) >> 20) | ((~(((inst & 0x80000000) >> 31) - 1)) & 0xFFFFF000));
}

template <unsigned int WIDTH>
constexpr normalized<WIDTH> simm(normalized<WIDTH> inst) {
  return normalize((inst & 0xF80) >> 7) | normalize(((inst & 0xFF000000) >> 24) << 5);
}

template <unsigned int WIDTH>
constexpr normalized<WIDTH> bimm(normalized<WIDTH> inst) {
  return normalized<WIDTH>(inst & 0x7F);
}

template <unsigned int WIDTH>
constexpr normalized<WIDTH> uimm(normalized<WIDTH> inst) {
  return normalized<WIDTH>(inst & 0x7F);
}

template <unsigned int WIDTH = 32>
using reg_if = std::function<normalized<WIDTH>(unsigned int)>;

template <unsigned int WIDTH>
class decoded;

class isa {
 public:
  using type = enum { RTYPE, ITYPE, STYPE, BTYPE, UTYPE, JTYPE };
  using extension = enum { BASE, M, A, F };
  using opcode = std::uint8_t;
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
  const decoded<WIDTH> decode(normalized<WIDTH> v, reg_if<WIDTH> accessor) const;
};

template <unsigned int WIDTH>
constexpr isa::opcode opcode(normalized<WIDTH> inst) {
  return isa::opcode(inst & 0x7F);
};

template <unsigned int WIDTH>
const decoded<WIDTH> isa::decode(normalized<WIDTH> v, reg_if<WIDTH> accessor) const {
  uint8_t opcode = ::opcode<WIDTH>(v);
  unsigned int rs1_id = ::rs1<WIDTH>(v);
  normalized<WIDTH> rs1_value = accessor(rs1_id);
  unsigned int rs2_id = ::rs2<WIDTH>(v);
  normalized<WIDTH> rs2_value = accessor(rs2_id);
  std::cout << std::to_string(rs1_id) << std::endl;
  switch (opcode) {
    case 0x33:
      return decoded<WIDTH>{opcode,    ::func3<WIDTH>(v), ::func7<WIDTH>(v),   rs1_value,
                            rs2_value, ::rd<WIDTH>(v),    normalized<WIDTH>{0}};
    case 0x13:
      return decoded<WIDTH>{opcode,    ::func3<WIDTH>(v), std::uint8_t{0}, rs1_value,
                            rs2_value, ::rd<WIDTH>(v),    ::iimm<WIDTH>(v)};
    default:
      return decoded<WIDTH>{0, std::uint8_t{0},     std::uint8_t{0}, normalized<WIDTH>{0}, normalized<WIDTH>{0},
                            0, normalized<WIDTH>{0}};
  }
}

template <unsigned int WIDTH = 32>
class decoded {
 public:
  decoded(std::uint8_t opcode, std::uint8_t func3, std::uint8_t func7, normalized<WIDTH> rs1, normalized<WIDTH> rs2,
          unsigned int rd, normalized<WIDTH> imm)
      : opcode(opcode), func3{func3}, func7{func7}, rs1{rs1}, rs2{rs2}, rd_id{rd}, imm{imm} {}

  decoded() : opcode{0} {}
  isa::opcode opcode;
  isa::extension ext;
  normalized<WIDTH> rs1;
  normalized<WIDTH> rs2;
  unsigned int rd_id;
  normalized<WIDTH> imm;
  std::uint8_t func3;
  std::uint8_t func7;
  bool mem_access;
  normalized<WIDTH> mem_address;
  bool branch_valid;
  normalized<WIDTH> branch_target_address;
  bool inc_pc;

  decoded(const decoded&) = default;
};
#endif  // ISA_HPP
