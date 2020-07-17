#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/tlm_quantumkeeper.h>

#include <systemc>
#include <tlm>

#include "bus.hpp"
#include "instruction.hpp"
#include "registers.hpp"

class matrix : public sc_core::sc_module {
  SC_HAS_PROCESS(matrix);

 public:
  void execute(instruction* ins) { ins->applied(m_registers, m_data); };

  void operating(){};
  void interrupted(tlm::tlm_generic_payload&, sc_core::sc_time&) {}
  matrix(const sc_core::sc_module_name& name) : sc_core::sc_module{name} {
    m_irq.register_b_transport(this, &matrix::interrupted);
    SC_THREAD(operating);
  };

 private:
  bus* m_code;
  bus* m_data;
  tlm_utils::simple_target_socket<matrix> m_irq;
  registers* m_registers;
  uint64_t m_pc;
  friend class matrix_system;
};

#endif  // MATRIX_HPP
