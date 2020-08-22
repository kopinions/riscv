#ifndef ISA_HPP
#define ISA_HPP
template <unsigned int WIDTH = 32>
class fields;

class isa {
 public:
  using type = enum { RTYPE, ITYPE, STYPE, BTYPE, UTYPE, JTYPE };
  using extension = enum { BASE, M, A, F };
  using opcode = std::uint8_t;
  using reg_idx = std::uint8_t;
  using func3 = std::uint8_t;
  using func7 = std::uint8_t;

  template <unsigned int WIDTH = 32>
  fields<WIDTH> unpack(bits::type<WIDTH> v) const {
    switch (type_of<WIDTH>(v)) {
      case isa::type::RTYPE:
        return fields<WIDTH>{RTYPE,         op<WIDTH>(v),  function3<WIDTH>(v), function7<WIDTH>(v),
                             rs1<WIDTH>(v), rs2<WIDTH>(v), rd<WIDTH>(v),        bits::type<WIDTH>{0}};
      case isa::type::ITYPE:
        return fields<WIDTH>{ITYPE,         op<WIDTH>(v),  function3<WIDTH>(v), func7{0},
                             rs1<WIDTH>(v), rs2<WIDTH>(v), rd<WIDTH>(v),        iimm<WIDTH>(v)};
      case isa::type::STYPE:
        return fields<WIDTH>{STYPE,         op<WIDTH>(v),  function3<WIDTH>(v), func7{0},
                             rs1<WIDTH>(v), rs2<WIDTH>(v), rd<WIDTH>(v),        simm<WIDTH>(v)};
      case isa::type::BTYPE:
        return fields<WIDTH>{BTYPE,         op<WIDTH>(v),  function3<WIDTH>(v), func7{0},
                             rs1<WIDTH>(v), rs2<WIDTH>(v), rd<WIDTH>(v),        bimm<WIDTH>(v)};
      case isa::type::UTYPE:
        return fields<WIDTH>{UTYPE,         op<WIDTH>(v),  function3<WIDTH>(v), func7{0},
                             rs1<WIDTH>(v), rs2<WIDTH>(v), rd<WIDTH>(v),        uimm<WIDTH>(v)};
      case isa::type::JTYPE:
        return fields<WIDTH>{JTYPE,         op<WIDTH>(v),  function3<WIDTH>(v), func7{0},
                             rs1<WIDTH>(v), rs2<WIDTH>(v), rd<WIDTH>(v),        jimm<WIDTH>(v)};
      default:
        return fields<WIDTH>{RTYPE, 0, func3{0}, func7{0}, reg_idx{0}, reg_idx{0}, reg_idx{0}, bits::type<WIDTH>{0}};
    }
  };

 private:
  template <unsigned int WIDTH>
  static inline reg_idx rs1(bits::type<WIDTH> inst) {
    return (reg_idx)((inst & 0x000F8000) >> 15);
  }

  template <unsigned int WIDTH>
  static inline reg_idx rs2(bits::type<WIDTH> inst) {
    return (reg_idx)((inst & 0x01F00000) >> 20);
  }

  template <unsigned int WIDTH>
  static inline func3 function3(bits::type<WIDTH> inst) {
    return (func3)((inst & 0x7000) >> 12);
  }

  template <unsigned int WIDTH>
  static inline func7 function7(bits::type<WIDTH> inst) {
    return (func7)((inst & 0xFE000000) >> 25);
  }

  template <unsigned int WIDTH>
  static inline reg_idx rd(bits::type<WIDTH> inst) {
    return reg_idx((inst & 0xF80) >> 7);
  }

  template <unsigned int WIDTH>
  static inline isa::opcode op(bits::type<WIDTH> inst) {
    return isa::opcode(inst & 0x7F);
  };

  template <unsigned int WIDTH>
  static inline bits::type<WIDTH> iimm(bits::type<WIDTH> inst) {
    bits::signed_type<WIDTH> imm = (bits::signed_type<WIDTH>)inst >> 20;
    return sign_extend<bits::signed_type<WIDTH>, 12>(imm);
  }

  template <unsigned int WIDTH>
  static inline bits::type<WIDTH> simm(bits::type<WIDTH> inst) {
    bits::signed_type<WIDTH> imm_h = ((bits::signed_type<WIDTH>)inst >> 25) << 5;
    bits::signed_type<WIDTH> imm_l = ((bits::signed_type<WIDTH>)inst >> 7) & 0x1F;
    return sign_extend<bits::signed_type<WIDTH>, 12>(imm_h | imm_l);
  }

  template <unsigned int WIDTH>
  static inline bits::type<WIDTH> bimm(bits::type<WIDTH> inst) {
    bits::signed_type<WIDTH> imm_s = ((bits::signed_type<WIDTH>)inst >> 30) << 12;
    bits::signed_type<WIDTH> imm_10_5 = (((bits::signed_type<WIDTH>)inst >> 25) & 0x3F) << 5;
    bits::signed_type<WIDTH> imm_4_1 = (((bits::signed_type<WIDTH>)inst >> 8) & 0xF) << 1;
    bits::signed_type<WIDTH> imm_11 = (((bits::signed_type<WIDTH>)inst >> 7) & 0x1) << 11;
    return sign_extend<bits::signed_type<WIDTH>, 13>((imm_s | imm_11 | imm_10_5 | imm_4_1));
  }

  template <unsigned int WIDTH>
  static inline bits::type<WIDTH> uimm(bits::type<WIDTH> inst) {
    bits::signed_type<WIDTH> imm = ((bits::signed_type<WIDTH>)inst >> 12);
    return sign_extend<bits::signed_type<WIDTH>, 20>((imm));
  }

  template <unsigned int WIDTH>
  static inline bits::type<WIDTH> jimm(bits::type<WIDTH> inst) {
    bits::signed_type<WIDTH> imm_20_0 = ((bits::signed_type<WIDTH>)inst >> 12);
    bits::signed_type<WIDTH> imm_20 = (imm_20_0 >> 19);
    bits::signed_type<WIDTH> imm_10_1 = (imm_20_0 >> 9) & 0x7F;
    bits::signed_type<WIDTH> imm_11 = (imm_20_0 >> 8) & 0x1;
    bits::signed_type<WIDTH> imm_19_12 = imm_20_0 & 0xFF;
    return sign_extend<bits::signed_type<WIDTH>, 21>((imm_20 | imm_19_12 | imm_11 | imm_10_1) << 1);
  }

  template <typename T, unsigned int R>
  static inline T sign_extend(const T x) {
    struct {
      T x : R;
    } s;
    return s.x = x;
  }

  template <unsigned int WIDTH = 32>
  static inline type type_of(bits::type<WIDTH> inst) {
    switch (op<WIDTH>(inst)) {
      case 0x33:
        return RTYPE;
      case 0x13:
        return ITYPE;
      case 0x23:
        return STYPE;
      case 0x63:
        return BTYPE;
      case 0x37:
      case 0x17:
        return UTYPE;
      case 0x6F:
        return JTYPE;
      default:
        return RTYPE;
    }
  };
};

template <unsigned int WIDTH>
class fields {
 public:
  fields(isa::type type, isa::opcode opcode, isa::func3 func3, isa::func7 func7, isa::reg_idx rs1, isa::reg_idx rs2,
         isa::reg_idx rd, bits::type<WIDTH> imm)
      : type{type}, opcode{opcode}, func3{func3}, func7{func7}, rs1{rs1}, rs2{rs2}, rd{rd}, imm{imm} {}

  fields() : opcode{0} {}
  isa::type type;
  isa::opcode opcode;
  isa::extension ext;
  isa::reg_idx rs1;
  isa::reg_idx rs2;
  isa::reg_idx rd;
  bits::type<WIDTH> imm;
  isa::func3 func3;
  isa::func7 func7;
  fields(const fields&) = default;
};
#endif  // ISA_HPP
