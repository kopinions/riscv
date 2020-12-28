#pragma once
#include <systemc.h>
#include <tlm_utils/simple_target_socket.h>

template <typename TYPES = tlm::tlm_base_protocol_types>
class fake_receiver : public sc_core::sc_module {
  SC_HAS_PROCESS(fake_receiver);
 public:
  using bw_interface_type = tlm::tlm_bw_transport_if<TYPES>;
  fake_receiver(
      const sc_core::sc_module_name& nm,
      std::function<void(bw_interface_type*, tlm::tlm_generic_payload&, tlm::tlm_phase& phase, sc_core::sc_time&)>&& f =
          [](bw_interface_type* bw_if, tlm::tlm_generic_payload&, tlm::tlm_phase& phase, sc_core::sc_time&) {
            return tlm::tlm_sync_enum::TLM_ACCEPTED;
          })
      : sc_module{nm}, m_hooked{std::move(f)} {
    inputs.register_nb_transport_fw(this, &fake_receiver::nb_transport_fw);
    SC_THREAD(operating);
  }

  tlm_utils::simple_target_socket<fake_receiver> inputs;

 private:
  void operating() {}
  tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& pld, tlm::tlm_phase& phase, sc_time& t) {
    m_hooked((dynamic_cast<bw_interface_type*>(inputs.get_base_port().get_interface())), pld, phase, t);
  }

  std::function<void(bw_interface_type*, tlm::tlm_generic_payload&, tlm::tlm_phase& phase, sc_core::sc_time&)> m_hooked;
};