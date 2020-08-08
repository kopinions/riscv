#ifndef FETCH_HPP
#define FETCH_HPP
#include <tlm_utils/simple_initiator_socket.h>

#include <iostream>
#include <systemc>
#include "registers.hpp"

template <unsigned int BITS = 32>
class fetch : public sc_core::sc_module {
 public:
  SC_HAS_PROCESS(fetch);
  tlm_utils::simple_initiator_socket<fetch> m_decode_initiator;
  tlm_utils::simple_initiator_socket<fetch> m_icache_initiator;
  fetch(const sc_core::sc_module_name& nm, std::shared_ptr<registers<BITS>> registers)
      : sc_core::sc_module{nm}, m_registers{registers} {
    SC_THREAD(operating);
  }

  [[noreturn]] void operating() {
    while (true) {
      uint32_t INSTR;
      tlm::tlm_generic_payload trans{};
      trans.set_command(tlm::TLM_READ_COMMAND);
      trans.set_data_ptr(reinterpret_cast<unsigned char*>(&INSTR));
      trans.set_data_length(4);
      trans.set_streaming_width(4);  // = data_length to indicate no streaming
      trans.set_byte_enable_ptr(0);  // 0 indicates unused
      trans.set_dmi_allowed(false);  // Mandatory initial value
      trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
      sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
      trans.set_address(m_registers->read(registers<BITS>::name::PC));
      m_icache_initiator->b_transport(trans, delay);
      std::cout << "fetch code" << INSTR << std::endl;
    }
  };

 private:
  std::shared_ptr<registers<BITS>> m_registers;
};
#endif  // FETCH_HPP
