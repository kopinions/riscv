#ifndef TIMER_HPP
#define TIMER_HPP
#include <tlm_utils/simple_initiator_socket.h>

#include <systemc>
class timer : public sc_core::sc_module {
 public:
  timer(const sc_core::sc_module_name &name) : sc_core::sc_module{name} {}
 private:
  tlm_utils::simple_initiator_socket<timer> irq_line;

};

#endif  // TIMER_HPP
