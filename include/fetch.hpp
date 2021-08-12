#ifndef FETCH_HPP
#define FETCH_HPP
#include <tlm_utils/simple_initiator_socket.h>

#include <iostream>
#include <systemc>

#include "commons/bits.hpp"
#include "registers.hpp"

static const char* FETCH_TYPE = "fetch";

template <unsigned int ADDR_WIDTH = 32, unsigned int DATA_WIDTH = 32>
class fetch : public sc_core::sc_module {
 public:
  SC_HAS_PROCESS(fetch);
  tlm_utils::simple_target_socket<fetch> m_decode_target;
  tlm_utils::simple_initiator_socket<fetch> m_icache_initiator;
  using instruction_type = typename bits::bits_helper<bits::normalize(DATA_WIDTH)>::type;
  using address_type = typename bits::bits_helper<bits::normalize(ADDR_WIDTH)>::type;

  fetch(const sc_core::sc_module_name& nm, std::shared_ptr<registers<ADDR_WIDTH>> registers)
      : sc_core::sc_module{nm}, m_registers{registers}, m_fetched{false}, m_fetched_event{"m_fetched"}, m_pc{0} {
    SC_THREAD(operating);
    m_decode_target.register_b_transport(this, &fetch::b_transport);
  }

  [[noreturn]] void operating() {
    while (true) {
      if (!m_fetched) {
        tlm::tlm_generic_payload trans{};
        trans.set_command(tlm::TLM_READ_COMMAND);
        trans.set_data_ptr(reinterpret_cast<unsigned char*>(&inst));
        trans.set_data_length(sizeof(inst));
        trans.set_streaming_width(sizeof(inst));  // = data_length to indicate no streaming
        trans.set_byte_enable_ptr(0);             // 0 indicates unused
        trans.set_dmi_allowed(false);             // Mandatory initial value
        trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
        sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
        trans.set_address(m_pc);
        m_icache_initiator->b_transport(trans, delay);
        SC_REPORT_INFO(FETCH_TYPE, ("Instruction get by fetch:  " + std::to_string(inst)).c_str());
        m_fetched = true;
        m_pc += 4;
        m_fetched_event.notify();
      } else {
        sc_core::wait(m_fetched_event);
      }
    }
  };

  void b_transport(tlm::tlm_generic_payload& payload, sc_core::sc_time& time) {
    while (!m_fetched) {
      sc_core::wait(m_fetched_event);
    }

    unsigned char* dataptr = payload.get_data_ptr();
    payload.set_response_status(tlm::TLM_OK_RESPONSE);
    memcpy(dataptr, &inst, sizeof(inst));
    m_fetched = false;
    m_fetched_event.notify();
  }

 private:
  std::shared_ptr<registers<ADDR_WIDTH>> m_registers;
  instruction_type inst;
  sc_core::sc_event m_fetched_event;
  unsigned int m_pc;
  bool m_fetched;
};
#endif  // FETCH_HPP
