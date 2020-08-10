#ifndef DECODE_HPP
#define DECODE_HPP
#include <tlm_utils/simple_target_socket.h>

#include <systemc>

#include "instruction.hpp"
#include "isa.hpp"

static const char* DECODE_TYPE = "decode";

template <unsigned int ADDR_WIDTH = 32, unsigned int DATA_WIDTH = 32>
class decode : public sc_core::sc_module {
 public:
  SC_HAS_PROCESS(decode);

 public:
  tlm_utils::simple_initiator_socket<decode> m_fetch_initiator;
  tlm_utils::simple_target_socket<decode> m_exec_target;

  using instruction_type = typename bits_helper<normalize(DATA_WIDTH)>::type;
  using address_type = typename bits_helper<normalize(ADDR_WIDTH)>::type;

  void b_transport(tlm::tlm_generic_payload& payload, sc_core::sc_time& delay) {
    if (!m_decoded) {
      sc_core::wait(m_decoded_event);
    }
    auto dataptr = payload.get_data_ptr();
    memcpy(dataptr, instruction, sizeof(*instruction));
    payload.set_response_status(tlm::TLM_OK_RESPONSE);

    m_decoded = false;
    m_decoded_event.notify();
  }

  decode(const sc_core::sc_module_name& nm) : sc_core::sc_module{nm} {
    m_exec_target.register_b_transport(this, &decode::b_transport);
    SC_THREAD(operating);
  }

  [[noreturn]] void operating() {
    while (true) {
      if (!m_decoded) {
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
        m_fetch_initiator->b_transport(trans, delay);
        *instruction = decoding(inst);
        SC_REPORT_INFO(DECODE_TYPE, ("Instruction get by decode:  " + std::to_string(inst)).c_str());
        m_decoded = true;
        m_decoded_event.notify();
      } else {
        sc_core::wait(m_decoded_event);
      }
    }
  }

 private:
  const instruction<DATA_WIDTH>& decoding(instruction_type inst) {
    const isa<>& isa = isa_router<DATA_WIDTH>(inst);
    return isa.from(inst);
  }
  sc_core::sc_event m_decoded_event;
  bool m_decoded;
  instruction<DATA_WIDTH>* instruction;
};
#endif  // DECODE_HPP
