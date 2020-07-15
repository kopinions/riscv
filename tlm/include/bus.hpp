#ifndef BUS_HPP
#define BUS_HPP
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/tlm_quantumkeeper.h>

#include <systemc>
class bus : public sc_core::sc_module {
  tlm_utils::simple_initiator_socket<bus> m_socket;
};
#endif  // BUS_HPP
