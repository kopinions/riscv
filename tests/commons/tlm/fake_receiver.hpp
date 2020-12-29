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
      std::function<tlm::tlm_sync_enum(bw_interface_type*, tlm::tlm_generic_payload&, tlm::tlm_phase& phase,
                                       sc_core::sc_time&)>&& f =
          [](bw_interface_type* bw_if, tlm::tlm_generic_payload&, tlm::tlm_phase& phase,
             sc_core::sc_time&) -> tlm::tlm_sync_enum { return tlm::tlm_sync_enum::TLM_ACCEPTED; })
      : sc_module{nm},
        m_hooked{std::move(f)},
        m_end_request_PEQ("end_request"),
        m_response_PEQ("end_response"),
        m_accept_delay(sc_time(10, SC_NS)) {
    inputs.register_nb_transport_fw(this, &fake_receiver::nb_transport_fw);
    SC_METHOD(end_request_method);
    sensitive << m_end_request_PEQ.get_event();
    dont_initialize();

    SC_METHOD(begin_response_method);
    sensitive << m_response_PEQ.get_event();
    dont_initialize();
  }

  tlm_utils::simple_target_socket<fake_receiver> inputs;

 private:
  void end_request_method() {
    for (auto* ptr = m_end_request_PEQ.get_next_transaction(); ptr; ptr = m_end_request_PEQ.get_next_transaction()) {
      sc_core::sc_time delay = sc_core::SC_ZERO_TIME;  // TODO: should set according to the target capability
      m_response_PEQ.notify(*ptr, delay);
      tlm::tlm_phase phase = tlm::END_REQ;
      auto status = inputs->nb_transport_bw(*ptr, phase, delay);
      switch (status) {
        case tlm::TLM_ACCEPTED: {
          break;
        }
        case tlm::TLM_UPDATED: {
          // TODO: this situation is invalid, initiator must receive data before update transaction
          break;
        }
        case tlm::TLM_COMPLETED: {
          // TODO: this situation is invalid, initiator must receive data before ending transaction
          break;
        }
      }
    }
  }

  void begin_response_method() {
    for (auto* ptr = m_response_PEQ.get_next_transaction(); ptr; ptr = m_response_PEQ.get_next_transaction()) {
      sc_core::sc_time delay = sc_core::SC_ZERO_TIME;  // TODO: should set according to the target capability
      tlm::tlm_phase phase = tlm::BEGIN_RESP;
      next_trigger(m_response_PEQ.get_event());
      // TODO: process the request and start response
      ptr->set_data_ptr((unsigned char*)"done");
      auto status = inputs->nb_transport_bw(*ptr, phase, delay);
      switch (status) {
        case tlm::TLM_ACCEPTED: {
          next_trigger(m_end_resp_rcvd_event);
          break;
        }
        case tlm::TLM_UPDATED: {
          // TODO: this situation is invalid, if the phase is done, the phase should be TLM_COMPLETED
          break;
        }
        case tlm::TLM_COMPLETED: {
          next_trigger(delay);
          break;
        }
      }
    }
  }

  tlm_utils::peq_with_get<tlm::tlm_generic_payload> m_end_request_PEQ;
  tlm_utils::peq_with_get<tlm::tlm_generic_payload> m_response_PEQ;

  tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& pld, tlm::tlm_phase& phase, sc_time& delay) {
    tlm::tlm_sync_enum status;
    switch (phase) {
      case tlm::BEGIN_REQ: {
        sc_core::sc_time PEQ_delay_time = delay + m_accept_delay;
        m_end_request_PEQ.notify(pld, PEQ_delay_time);
        status = tlm::TLM_ACCEPTED;
        break;
      }
      case tlm::END_RESP: {
        m_end_resp_rcvd_event.notify(delay);
        status = tlm::TLM_COMPLETED;
      }
      case tlm::END_REQ:
      case tlm::BEGIN_RESP: {
        std::cerr << "error";
        status = tlm::TLM_ACCEPTED;
        break;
      }
    }
    return m_hooked((dynamic_cast<bw_interface_type*>(inputs.get_base_port().get_interface())), pld, phase, delay);
  }

  std::function<tlm::tlm_sync_enum(bw_interface_type*, tlm::tlm_generic_payload&, tlm::tlm_phase& phase,
                                   sc_core::sc_time&)>
      m_hooked;
  const sc_time m_accept_delay;
  sc_event m_end_resp_rcvd_event;
};