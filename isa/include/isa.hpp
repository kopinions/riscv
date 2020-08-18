#ifndef ISA_HPP
#define ISA_HPP
template <unsigned int WIDTH = 32>
class fields;

class isa {
 public:
  using type = enum { RTYPE, ITYPE, STYPE, BTYPE, UTYPE, JTYPE };
  using extension = enum { BASE, M, A, F };
  using opcode_t = std::uint8_t;
  using reg_idx = std::uint8_t;
  using func3 = std::uint8_t;
  using func7 = std::uint8_t;

  template <unsigned int WIDTH = 32>
  const fields<WIDTH> unpack(normalized<WIDTH> v) const {
    switch (type_of<WIDTH>(v)) {
      case isa::type::RTYPE:
        return fields<WIDTH>{RTYPE,         opcode<WIDTH>(v), function3<WIDTH>(v), function7<WIDTH>(v),
                             rs1<WIDTH>(v), rs2<WIDTH>(v),    rd<WIDTH>(v),        normalized<WIDTH>{0}};
      case isa::type::ITYPE:
        return fields<WIDTH>{ITYPE,         opcode<WIDTH>(v), function3<WIDTH>(v), func7{0},
                             rs1<WIDTH>(v), rs2<WIDTH>(v),    rd<WIDTH>(v),        iimm<WIDTH>(v)};
      default:
        return fields<WIDTH>{RTYPE, 0, func3{0}, func7{0}, reg_idx{0}, reg_idx{0}, reg_idx{0}, normalized<WIDTH>{0}};
    }
  };

 private:
  template <unsigned int WIDTH>
  static inline reg_idx rs1(normalized<WIDTH> inst) {
    return (reg_idx)((inst & 0x000F8000) >> 15);
  }

  template <unsigned int WIDTH>
  static inline reg_idx rs2(normalized<WIDTH> inst) {
    return (reg_idx)((inst & 0x01F00000) >> 20);
  }

  template <unsigned int WIDTH>
  static inline func3 function3(normalized<WIDTH> inst) {
    return (func3)((inst & 0x700) >> 12);
  }

  template <unsigned int WIDTH>
  static inline func7 function7(normalized<WIDTH> inst) {
    return (func7)((inst & 0xFE000000) >> 25);
  }

  template <unsigned int WIDTH>
  static inline reg_idx rd(normalized<WIDTH> inst) {
    return reg_idx((inst & 0xF80) >> 7);
  }

  template <unsigned int WIDTH>
  static inline isa::opcode_t opcode(normalized<WIDTH> inst) {
    return isa::opcode_t(inst & 0x7F);
  };

  template <unsigned int WIDTH>
  static inline normalized<WIDTH> iimm(normalized<WIDTH> inst) {
    // sign = (inst & 0x80000000) >> 31
    // mask = ~(sign-1) & 0xFFFFF000;
    // imm = imm[12:0] | mask
    unsigned int sign = (inst & 0x80000000) >> 31;
    unsigned int imm = (inst & 0xFFF00000) >> 20;
    return normalized<WIDTH>(imm | ((~(sign - 1)) & 0xFFFFF000));
  }

  template <unsigned int WIDTH = 32>
  static inline type type_of(normalized<WIDTH> inst) {
    switch (opcode<WIDTH>(inst)) {
      case 0x33:
        return RTYPE;
      case 0x13:
        return ITYPE;
      default:
        return RTYPE;
    }
  };
};

template <unsigned int WIDTH>
class fields {
 public:
  fields(isa::type type, isa::opcode_t opcode, isa::func3 func3, isa::func7 func7, isa::reg_idx rs1, isa::reg_idx rs2,
         isa::reg_idx rd, normalized<WIDTH> imm)
      : type{type}, opcode{opcode}, func3{func3}, func7{func7}, rs1{rs1}, rs2{rs2}, rd{rd}, imm{imm} {}

  fields() : opcode{0} {}
  isa::type type;
  isa::opcode_t opcode;
  isa::extension ext;
  isa::reg_idx rs1;
  isa::reg_idx rs2;
  isa::reg_idx rd;
  normalized<WIDTH> imm;
  isa::func3 func3;
  isa::func7 func7;
  fields(const fields&) = default;
};
#endif  // ISA_HPP
