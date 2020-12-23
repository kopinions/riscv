#pragma once
#include <systemc.h>
#include <tlm_utils/simple_target_socket.h>

class fake_receiver : public sc_core::sc_module {
  SC_HAS_PROCESS(fake_receiver);

 public:
  fake_receiver(
      const sc_core::sc_module_name& nm, std::function<void(tlm::tlm_generic_payload&, sc_core::sc_time&)>&& f =
                                             [](tlm::tlm_generic_payload&, sc_core::sc_time&) {})
      : sc_module{nm}, m_hooked{std::move(f)} {
    inputs.register_b_transport(this, &fake_receiver::b_transport);
    SC_THREAD(operating);
  }

  tlm_utils::simple_target_socket<fake_receiver> inputs;

 private:
  void operating() {}
  void b_transport(tlm::tlm_generic_payload& pld, sc_time& t) { m_hooked(pld, t); }

  std::function<void(tlm::tlm_generic_payload&, sc_core::sc_time&)> m_hooked;
};