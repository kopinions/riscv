#pragma once

#include <systemc.h>
#include <tlm_utils/simple_initiator_socket.h>

class fake_sender : public sc_core::sc_module {
  SC_HAS_PROCESS(fake_sender);

 public:
  fake_sender(
      const sc_core::sc_module_name& nm, std::function<void(tlm_utils::simple_initiator_socket<fake_sender>&)> f =
                                             [](tlm_utils::simple_initiator_socket<fake_sender>&) {})
      : sc_module{nm}, m_hooked{f} {
    SC_THREAD(operating);
  }

  tlm_utils::simple_initiator_socket<fake_sender> outputs;

 private:
  void operating() { m_hooked(outputs); }
  std::function<void(tlm_utils::simple_initiator_socket<fake_sender>&)> m_hooked;
};