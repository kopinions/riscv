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
      std::cout << "test" << std::endl;
      wait();
    }
  };

 private:
  std::shared_ptr<registers<BITS>> m_registers;
};
#endif  // FETCH_HPP
