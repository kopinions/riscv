#ifndef ICACHE_HPP
#define ICACHE_HPP
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

#include <systemc>

class icache : public sc_core::sc_module {
 public:
  SC_HAS_PROCESS(icache);
  tlm_utils::simple_target_socket<icache> m_fetch_target;
  tlm_utils::simple_initiator_socket<icache> m_code_initiator;

  icache(const sc_core::sc_module_name& nm) : sc_core::sc_module(nm) { SC_THREAD(operating); }

  [[noreturn]] void operating() {
    while (true) {
      sleep(1);
    }
  };
};
#endif  // ICACHE_HPP
