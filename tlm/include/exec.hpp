#ifndef EXEC_HPP
#define EXEC_HPP
#include <tlm_utils/simple_initiator_socket.h>

#include <systemc>

class exec : public sc_core::sc_module {
 public:
  tlm_utils::simple_initiator_socket<exec> m_decode_initiator;
  tlm_utils::simple_initiator_socket<exec> m_dcache_initiator;

  exec(const sc_core::sc_module_name& nm) : sc_core::sc_module(nm) {}

};
#endif  // EXEC_HPP
