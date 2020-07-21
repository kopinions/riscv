#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/tlm_quantumkeeper.h>

#include <systemc>
#include <tlm>

#include "bus.hpp"
#include "dcache.hpp"
#include "decode.hpp"
#include "exec.hpp"
#include "fetch.hpp"
#include "icache.hpp"
#include "instruction.hpp"
#include "mm.hpp"
#include "registers.hpp"

template <unsigned int ADDR_WIDTH = 32, unsigned int DATA_WIDTH = 32>
class matrix : public sc_core::sc_module {
 public:
  void interrupted(tlm::tlm_generic_payload&, sc_core::sc_time&) {}

  matrix(const sc_core::sc_module_name& name)
      : sc_core::sc_module{name},
        m_icache{"icache"},
        m_fetch{"fetch"},
        m_decode{"decode"},
        m_exec{"exec"},
        m_dcache{"dcacne"},
        m_registers{"registers"} {
    m_irq.register_b_transport(this, &matrix::interrupted);

    m_fetch.m_icache_initiator.bind(m_icache.m_fetch_target);
    m_fetch.m_decode_initiator.bind(m_decode.m_fetch_target);
    m_exec.m_decode_initiator.bind(m_decode.m_exec_target);
    m_exec.m_dcache_initiator.bind(m_dcache.m_exec_target);
  };

 private:

  tlm_utils::simple_target_socket<matrix> m_irq;
  icache m_icache;
  fetch m_fetch;
  decode m_decode;
  exec m_exec;
  dcache m_dcache;
  registers<DATA_WIDTH> m_registers;
  friend class matrix_system;
};

#endif  // MATRIX_HPP
