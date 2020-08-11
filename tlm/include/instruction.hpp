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
  virtual void applied(registers<WIDTH>& regs, mm* mm) const {};
};

template <unsigned int WIDTH = 32>
class rtype : public instruction<WIDTH> {
 public:
  void applied(registers<WIDTH>& regs, mm* mm) const override {}

 private:
};

template <unsigned int WIDTH = 32>
class itype : public instruction<WIDTH> {
 public:
  void applied(registers<WIDTH>& regs, mm* mm) const override {}
};

template <unsigned int WIDTH = 32>
class stype : public instruction<WIDTH> {
 public:
  void applied(registers<WIDTH>& regs, mm* mm) const override {}
};

template <unsigned int WIDTH = 32>
class btype : public instruction<WIDTH> {
 public:
  void applied(registers<WIDTH>& regs, mm* mm) const override {}
};

template <unsigned int WIDTH = 32>
class utype : public instruction<WIDTH> {
 public:
  void applied(registers<WIDTH>& regs, mm* mm) const override {}
};

template <unsigned int WIDTH = 32>
class jtype : public instruction<WIDTH> {
 public:
  void applied(registers<WIDTH>& regs, mm* mm) override {}
};

template <unsigned int WIDTH = 32>
std::unique_ptr<instruction<WIDTH>> create(decoded<WIDTH> decoded) {
  switch (isa::type_of<WIDTH>(decoded)) {
    case isa::type::RTYPE:
      return std::move(std::make_unique<rtype<WIDTH>>());
    case isa::type::ITYPE:
      return std::move(std::make_unique<itype<WIDTH>>());
    default:
      return std::move(std::make_unique<rtype<WIDTH>>());
  }
};

#endif  // INSTRUCTION_HPP
