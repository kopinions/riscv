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
  virtual void applied(registers<WIDTH>* regs, mm* mm) const = 0;
  virtual ~instruction() = default;
};

template <unsigned int WIDTH = 32>
class rtype : public instruction<WIDTH> {
 public:
  rtype(isa::opcode opcode, std::uint8_t func3, std::uint8_t func7, normalized<WIDTH> rs1, normalized<WIDTH> rs2,
        unsigned int rd_id)
      : m_opcode{opcode}, m_func3{func3}, m_func7{func7}, m_rs1{rs1}, m_rs2{rs2}, m_rd_id{rd_id} {}
  void applied(registers<WIDTH>* regs, mm* mm) const override {
    switch ((std::uint32_t{m_func7} << 3) | m_func3) {
      case 0x000:
        regs->write(m_rd_id, m_rs1 + m_rs2);
        std::cout << std::to_string(regs->read(m_rd_id)) << std::endl;
        break;
      default:
        break;
    }
  }

 private:
  isa::opcode m_opcode;
  std::uint8_t m_func3;
  std::uint8_t m_func7;
  normalized<WIDTH> m_rs1;
  normalized<WIDTH> m_rs2;
  unsigned int m_rd_id;
};

template <unsigned int WIDTH = 32>
class itype : public instruction<WIDTH> {
 public:
  void applied(registers<WIDTH>* regs, mm* mm) const override {}
};

template <unsigned int WIDTH = 32>
class stype : public instruction<WIDTH> {
 public:
  void applied(registers<WIDTH>* regs, mm* mm) const override {}
};

template <unsigned int WIDTH = 32>
class btype : public instruction<WIDTH> {
 public:
  void applied(registers<WIDTH>* regs, mm* mm) const override {}
};

template <unsigned int WIDTH = 32>
class utype : public instruction<WIDTH> {
 public:
  void applied(registers<WIDTH>* regs, mm* mm) const override {}
};

template <unsigned int WIDTH = 32>
class jtype : public instruction<WIDTH> {
 public:
  void applied(registers<WIDTH>* regs, mm* mm) override {}
};

template <unsigned int WIDTH = 32>
std::unique_ptr<instruction<WIDTH>> create(decoded<WIDTH> decoded) {
  switch (isa::type_of<WIDTH>(decoded)) {
    case isa::type::RTYPE:
      return std::move(std::make_unique<rtype<WIDTH>>(decoded.opcode, decoded.func3, decoded.func7, decoded.rs1,
                                                      decoded.rs2, decoded.rd_id));
    case isa::type::ITYPE:
      return std::move(std::make_unique<itype<WIDTH>>());
    default:
      return std::move(std::make_unique<rtype<WIDTH>>(decoded.opcode, decoded.func3, decoded.func7, decoded.rs1,
                                                      decoded.rs2, decoded.rd_id));
  }
};

#endif  // INSTRUCTION_HPP
