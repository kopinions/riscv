#ifndef MATRIX_TOP_HPP
#define MATRIX_TOP_HPP
#include <systemc>
#include <tlm>

class matrix_top : public sc_core::sc_module {
 public:
  matrix_top(){};

 protected:
  tlm_utils::simple_initiator_socket<CPU> instr_bus;
  tlm_utils::simple_target_socket<CPU> irq_line_socket;
  bus* m_inst_bus;
  bus* m_data_bus;
};

#endif  // MATRIX_TOP_HPP
