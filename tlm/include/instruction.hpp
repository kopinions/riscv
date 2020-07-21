#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP
#include <tlm_utils/simple_initiator_socket.h>

#include <systemc>

#include "mm.hpp"
#include "registers.hpp"

template <unsigned int INSTRUCTION_WIDTH = 32>
class instruction : public sc_core::sc_module {
 public:
  virtual void applied(registers<INSTRUCTION_WIDTH>& regs, mm* mm) = 0;
};
#endif  // INSTRUCTION_HPP
