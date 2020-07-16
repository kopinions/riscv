#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP
#include <systemc>

#include "registers.hpp"
#include "bus.hpp"

class instruction : public sc_core::sc_module {
 public:
  virtual void applied(registers* regs, bus* data) = 0;
};
#endif  // INSTRUCTION_HPP
