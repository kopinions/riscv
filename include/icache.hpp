#ifndef ICACHE_HPP
#define ICACHE_HPP
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

#include <map>
#include <systemc>

#include "commons/bits.hpp"

static const char* ICACHE_TYPE = "icache";
template <unsigned int ADDR_WIDTH = 32, unsigned int DATA_WIDTH = 32>
class icache : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<icache> m_fetch_target;
  tlm_utils::simple_initiator_socket<icache> m_code_initiator;
  using instruction_type = typename bits::bits_helper<bits::normalize(DATA_WIDTH)>::type;
  using address_type = typename bits::bits_helper<bits::normalize(ADDR_WIDTH)>::type;

  void b_transport(tlm::tlm_generic_payload& fetch, sc_core::sc_time&) {
    instruction_type inst;

    tlm::tlm_generic_payload trans{};
    trans.set_command(tlm::TLM_READ_COMMAND);
    trans.set_data_ptr(reinterpret_cast<unsigned char*>(&inst));
    trans.set_data_length(sizeof(inst));
    trans.set_streaming_width(4);  // = data_length to indicate no streaming
    trans.set_byte_enable_ptr(0);  // 0 indicates unused
    trans.set_address(fetch.get_address());
    trans.set_dmi_allowed(false);  // Mandatory initial value
    trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
    m_code_initiator->b_transport(trans, delay);
    SC_REPORT_INFO(ICACHE_TYPE, ("Instruction get by icache:  " + std::to_string(inst)).c_str());
    auto fetchptr = fetch.get_data_ptr();
    memcpy(fetchptr, &inst, sizeof(inst));
    //    fetch.set_response_status(tlm::TLM_OK_RESPONSE);
  }

  icache(const sc_core::sc_module_name& nm) : sc_core::sc_module{nm}, m_cache{} {
    m_fetch_target.register_b_transport(this, &icache::b_transport);
  }

 private:
  std::map<address_type, instruction_type> m_cache;
};
#endif  // ICACHE_HPP
