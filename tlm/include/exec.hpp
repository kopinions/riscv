#ifndef EXEC_HPP
#define EXEC_HPP
#include <tlm_utils/simple_initiator_socket.h>

#include <systemc>

#include "alu.hpp"
static const char* EXEC_TYPE = "exec";
template <unsigned int ADDR_WIDTH = 32, unsigned int DATA_WIDTH = 32>
class exec : public sc_core::sc_module {
 public:
  SC_HAS_PROCESS(exec);
  tlm_utils::simple_initiator_socket<exec> m_decode_initiator;
  tlm_utils::simple_initiator_socket<exec> m_dcache_initiator;

  exec(const sc_core::sc_module_name& nm, std::shared_ptr<registers<ADDR_WIDTH>> registers)
      : sc_core::sc_module(nm), m_execed{false}, m_execed_event{"execed"}, m_registers{registers} {
    SC_THREAD(operating)
  }

 private:
  [[noreturn]] void operating() {
    while (true) {
      if (!m_execed) {
        tlm::tlm_generic_payload trans{};
        instruction<DATA_WIDTH> m_decoded;
        trans.set_command(tlm::TLM_READ_COMMAND);
        trans.set_data_ptr(reinterpret_cast<unsigned char*>(&m_decoded));
        trans.set_streaming_width(sizeof(m_decoded));  // = data_length to indicate no streaming
        trans.set_byte_enable_ptr(0);                  // 0 indicates unused
        trans.set_dmi_allowed(false);                  // Mandatory initial value
        trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
        sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
        m_decode_initiator->b_transport(trans, delay);
          SC_REPORT_INFO(EXEC_TYPE, ("Instruction get by exec: " + std::to_string(m_decoded.of())).c_str());
        registers<DATA_WIDTH>* regs = m_registers.get();
        const alu<DATA_WIDTH>& al = alu<DATA_WIDTH>();
        const typename instruction<DATA_WIDTH>::result result = al.evaluate(m_decoded);
        // TODO: write back or mem access

        m_execed = true;
        m_execed_event.notify();
      } else {
        sc_core::wait(m_execed_event);
      }
    }
  }

 private:
  bool m_execed;
  std::shared_ptr<registers<DATA_WIDTH>> m_registers;
  sc_core::sc_event m_execed_event;
};
#endif  // EXEC_HPP
