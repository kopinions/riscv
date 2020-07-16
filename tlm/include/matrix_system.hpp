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
#include "binder.hpp"



class matrix_system : public sc_core::sc_module {
 public:
  matrix_system(const sc_core::sc_module_name& name) noexcept : sc_core::sc_module{name} { bind<matrix, timer>(m_matrix, m_timer); };

 private:
  matrix* m_matrix;
  timer* m_timer;
//  template <typename Initializer, typename Target>
//  friend void bind(Initializer, Target);
};

#endif  // MATRIX_SYSTEM_HPP
