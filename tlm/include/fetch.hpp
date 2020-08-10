#ifndef FETCH_HPP
#define FETCH_HPP
#include <tlm_utils/simple_initiator_socket.h>

#include <iostream>
#include <systemc>

#include "registers.hpp"

static const char* FETCH_TYPE = "fetch";

template <unsigned int ADDR_WIDTH = 32, unsigned int DATA_WIDTH = 32>
class fetch : public sc_core::sc_module {
 public:
  SC_HAS_PROCESS(fetch);
  tlm_utils::simple_initiator_socket<fetch> m_decode_initiator;
  tlm_utils::simple_initiator_socket<fetch> m_icache_initiator;
  using instruction_type = typename bits_helper<normalize(DATA_WIDTH)>::type;
  using address_type = typename registers<ADDR_WIDTH>::type;
  fetch(const sc_core::sc_module_name& nm, std::shared_ptr<registers<ADDR_WIDTH>> registers)
      : sc_core::sc_module{nm}, m_registers{registers} {
    SC_THREAD(operating);
  }

  [[noreturn]] void operating() {
    while (true) {
      instruction_type inst;
      tlm::tlm_generic_payload trans{};
      trans.set_command(tlm::TLM_READ_COMMAND);
      trans.set_data_ptr(reinterpret_cast<unsigned char*>(&inst));
      trans.set_data_length(sizeof(inst));
      trans.set_streaming_width(sizeof(inst));  // = data_length to indicate no streaming
      trans.set_byte_enable_ptr(0);             // 0 indicates unused
      trans.set_dmi_allowed(false);             // Mandatory initial value
      trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
      sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
      trans.set_address(m_registers->read(registers<ADDR_WIDTH>::name::PC));
      m_icache_initiator->b_transport(trans, delay);
      SC_REPORT_INFO(FETCH_TYPE, ("Instruction get by fetch:  " + std::to_string(inst)).c_str());
    }
  };

 private:
  std::shared_ptr<registers<ADDR_WIDTH>> m_registers;
};
#endif  // FETCH_HPP
