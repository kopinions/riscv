#ifndef MATRIX_TOP_HPP
#define MATRIX_TOP_HPP
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/tlm_quantumkeeper.h>

#include <systemc>
#include <tlm>

class matrix : public sc_core::sc_module {
 public:
  matrix(sc_core::sc_module_name name) : sc_core::sc_module{name} { SC_THREAD(operating); };
  void execute(instruction* ins) { ins.applied(m_registers, m_data); };
  void operating(){

  };

 protected:
  bus m_code;
  bus m_data;
  tlm_utils::simple_target_socket<matrix> m_irq;
  registers* m_registers;
  uint64_t m_pc;
};

#endif  // MATRIX_TOP_HPP
