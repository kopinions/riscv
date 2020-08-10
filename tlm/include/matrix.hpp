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
#include <memory>

template <unsigned int ADDR_WIDTH = 32, unsigned int DATA_WIDTH = 32>
class matrix : public sc_core::sc_module {
 public:
  void interrupted(tlm::tlm_generic_payload&, sc_core::sc_time&) {}

  matrix(const sc_core::sc_module_name& name)
      : sc_core::sc_module{name},
        m_registers(std::make_shared<registers<DATA_WIDTH>>("registers")),
        m_icache{"icache"},
        m_fetch{"fetch", m_registers},
        m_decode{"decode"},
        m_exec{"exec"},
        m_dcache{"dcacne"} {
    m_irq.register_b_transport(this, &matrix::interrupted);

    m_fetch.m_icache_initiator.bind(m_icache.m_fetch_target);
    m_fetch.m_decode_initiator.bind(m_decode.m_fetch_target);
    m_exec.m_decode_initiator.bind(m_decode.m_exec_target);
    m_exec.m_dcache_initiator.bind(m_dcache.m_exec_target);
  };

 private:
  std::shared_ptr<registers<DATA_WIDTH>> m_registers;
  tlm_utils::simple_target_socket<matrix> m_irq;
  icache<ADDR_WIDTH, DATA_WIDTH> m_icache;
  fetch<ADDR_WIDTH,DATA_WIDTH> m_fetch;
  decode m_decode;
  exec m_exec;
  dcache m_dcache;
  friend class matrix_system;
};

#endif  // MATRIX_HPP
