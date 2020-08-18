#ifndef DECODE_HPP
#define DECODE_HPP
#include <tlm_utils/simple_target_socket.h>

#include <isa.hpp>
#include <systemc>

#include "instruction.hpp"

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
    memcpy(dataptr, reinterpret_cast<unsigned char*>(m_decoded_instruction), sizeof(*m_decoded_instruction));

    payload.set_response_status(tlm::TLM_OK_RESPONSE);
    m_decoded = false;
    m_decoded_event.notify();
  }

  decode(const sc_core::sc_module_name& nm, std::shared_ptr<registers<ADDR_WIDTH>> registers)
      : sc_core::sc_module{nm}, m_registers{registers}, m_decoded{false}, m_decoded_event{"m_decoded"} {
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
        isa riscv{};
        const fields<32>& fields = riscv.unpack(inst);
        instruction<DATA_WIDTH> d = instruction<DATA_WIDTH>{
            fields.opcode, fields.func3, fields.func7, m_registers->read(fields.rs1), m_registers->read(fields.rs2),
            fields.rd,     fields.imm};

        m_decoded_instruction = &d;
        SC_REPORT_INFO(DECODE_TYPE, ("Instruction get by decode:  " + std::to_string(inst)).c_str());
        m_decoded = true;
        m_decoded_event.notify();
      } else {
        sc_core::wait(m_decoded_event);
      }
    }
  }

 private:
  std::shared_ptr<registers<DATA_WIDTH>> m_registers;
  sc_core::sc_event m_decoded_event;
  bool m_decoded;
  instruction<DATA_WIDTH>* m_decoded_instruction;
};
#endif  // DECODE_HPP
