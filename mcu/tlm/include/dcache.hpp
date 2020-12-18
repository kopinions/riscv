#ifndef DCACHE_HPP
#define DCACHE_HPP
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

#include <systemc>

class dcache : public sc_core::sc_module {
 public:
  tlm_utils::simple_target_socket<dcache> m_exec_target;
  tlm_utils::simple_initiator_socket<dcache> m_data_initiator;
  dcache(const sc_core::sc_module_name& nm) : sc_core::sc_module(nm) {}
};
#endif  // DCACHE_HPP
