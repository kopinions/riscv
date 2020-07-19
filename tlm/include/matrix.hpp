#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/tlm_quantumkeeper.h>

#include <systemc>
#include <tlm>

#include "bus.hpp"
#include "instruction.hpp"
#include "mm.hpp"
#include "registers.hpp"

template <unsigned int INSTRUCTION_WIDTH = 32>
class matrix : public sc_core::sc_module {
  SC_HAS_PROCESS(matrix);

 public:
  void execute(instruction<INSTRUCTION_WIDTH>* ins) { ins->applied(m_registers, m_mm); };

  void operating(){
    while (true) {
      
    }
  };
  void interrupted(tlm::tlm_generic_payload&, sc_core::sc_time&) {}
  matrix(const sc_core::sc_module_name& name) : sc_core::sc_module{name} {
    m_irq.register_b_transport(this, &matrix::interrupted);
    m_mm = new mm{"m_mm"};
    m_data.bind(m_mm->m_target);
    SC_THREAD(operating);
  };

 private:
  tlm_utils::simple_initiator_socket<matrix> m_code;
  tlm_utils::simple_initiator_socket<matrix> m_data;
  tlm_utils::simple_target_socket<matrix> m_irq;
  registers* m_registers;
  mm* m_mm;
  uint64_t m_pc;
  friend class matrix_system;
};

#endif  // MATRIX_HPP
