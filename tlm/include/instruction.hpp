#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP
#include <tlm_utils/simple_initiator_socket.h>

#include <memory>
#include <systemc>

#include "mm.hpp"
#include "registers.hpp"

template <unsigned int WIDTH = 32>
class instruction {
 public:
  isa::type type;
  isa::opcode opcode;
  isa::extension ext;
  bits::type<WIDTH> rs1;
  bits::type<WIDTH> rs2;
  isa::reg_idx rd;
  bits::type<WIDTH> imm;
  isa::func3 func3;
  isa::func7 func7;
  instruction() {}
  explicit instruction(isa::opcode opcode, isa::func3 func3, isa::func7 func7, bits::type<WIDTH> rs1,
                       bits::type<WIDTH> rs2, isa::reg_idx rd, bits::type<WIDTH> imm)
      : opcode(opcode), func3{func3}, func7{func7}, rs1{rs1}, rs2{rs2}, rd{rd}, imm{imm} {}
  instruction(const instruction&) = default;
  class result {
   public:
    bits::type<WIDTH> flags;
    bits::type<WIDTH> res;
    bool wb;
    unsigned int rd_id;
    bool mem_access;
    bits::type<WIDTH> mem_address;
    bool branch_valid;
    bits::type<WIDTH> branch_target_address;
    bool inc_pc;
    bool mem_write;
    result() {}
  };

  virtual ~instruction() = default;
};

template <unsigned int WIDTH = 32>
class alu {
 public:
  virtual typename instruction<WIDTH>::result evaluate(instruction<WIDTH> inst) const {
    typename instruction<WIDTH>::result res = typename instruction<WIDTH>::result{};
    switch ((std::uint32_t{inst.func7} << 3) | inst.func3) {
      case 0x000: {
        res.wb = true;
        res.res = inst.rs1 + inst.rs2;
        break;
      }
      default:
        break;
    }
    return res;
  };
};

#endif  // INSTRUCTION_HPP
