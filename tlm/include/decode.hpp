#ifndef DECODE_HPP
#define DECODE_HPP
#include <tlm_utils/simple_target_socket.h>

#include <systemc>

class decode : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<decode> m_fetch_target;
  tlm_utils::simple_target_socket<decode> m_exec_target;

  decode(const sc_core::sc_module_name& nm) : sc_core::sc_module(nm) {}
};
#endif  // DECODE_HPP
