#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP
#include <tlm_utils/simple_initiator_socket.h>

#include <memory>
#include <systemc>

#include "mm.hpp"
#include "registers.hpp"

template <isa::type TYPE, unsigned int WIDTH>
class inferences;

template <unsigned int WIDTH>
class alu;

template <unsigned int WIDTH = 32>
class instruction {
 public:
  friend class alu<WIDTH>;
  friend class inferences<isa::type::RTYPE, WIDTH>;
  friend class inferences<isa::type::ITYPE, WIDTH>;
  friend class inferences<isa::type::STYPE, WIDTH>;
  friend class inferences<isa::type::UTYPE, WIDTH>;
  friend class inferences<isa::type::BTYPE, WIDTH>;
  friend class inferences<isa::type::JTYPE, WIDTH>;

  using type = enum { ADD, ADDI, UNSUPPORTED };
  type typ;
  bits::type<WIDTH> rs1;
  bits::type<WIDTH> rs2;
  isa::reg_idx rd;
  bits::type<WIDTH> imm;
  instruction() {}
  explicit instruction(type type, bits::type<WIDTH> rs1, bits::type<WIDTH> rs2, isa::reg_idx rd, bits::type<WIDTH> imm)
      : typ(type), rs1{rs1}, rs2{rs2}, rd{rd}, imm{imm} {}
  instruction(const instruction&) = default;

  type of() const { return typ; }

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

#endif  // INSTRUCTION_HPP
