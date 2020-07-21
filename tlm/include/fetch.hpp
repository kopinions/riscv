#ifndef FETCH_HPP
#define FETCH_HPP
#include <tlm_utils/simple_initiator_socket.h>

#include <iostream>
#include <systemc>

class fetch : public sc_core::sc_module {
 public:
  SC_HAS_PROCESS(fetch);
  tlm_utils::simple_initiator_socket<fetch> m_decode_initiator;
  tlm_utils::simple_initiator_socket<fetch> m_icache_initiator;
  fetch(const sc_core::sc_module_name& nm) : sc_core::sc_module{nm} { SC_THREAD(operating); }

  [[noreturn]] void operating() {
    std::cout << "test" << std::endl;
    while (true) {
      sleep(1);
    }
  };
};
#endif  // FETCH_HPP
