#ifndef MATRIX_SYSTEM_HPP
#define MATRIX_SYSTEM_HPP
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/tlm_quantumkeeper.h>

#include <systemc>
#include <tlm>

#include "bus.hpp"
#include "instruction.hpp"
#include "matrix.hpp"
#include "registers.hpp"
#include "timer.hpp"

class matrix_system : public sc_core::sc_module {
 public:
  matrix_system(const sc_core::sc_module_name& name) noexcept : sc_core::sc_module{name} {
    m_matrix = new matrix{"matrix"};
    m_timer = new timer{"timer"};
    m_timer->irq_line.bind(m_matrix->m_irq);
  };

  ~matrix_system() {
    delete m_matrix;
    delete m_timer;
  }

 private:
  matrix* m_matrix;
  timer* m_timer;
};

#endif  // MATRIX_SYSTEM_HPP
