
template <typename RECORDABLE, typename TYPES>
inbound<RECORDABLE, TYPES>::inbound(const sc_module_name& name)
    : RECORDABLE(name),
      m_requests_to_end("end_request"),
      m_responses_to_begin("begin_response"),
      m_accept_delay(sc_time(10, SC_NS)) {
  SC_METHOD(end_request_method);
  sensitive << m_requests_to_end.get_event();
  dont_initialize();

  SC_METHOD(begin_response_method);
  sensitive << m_responses_to_begin.get_event();
  dont_initialize();
}

template <typename RECORDABLE, typename TYPES>
inbound<RECORDABLE, TYPES>::inbound() : inbound(sc_core::sc_gen_unique_name("inbound")) {}
template <typename RECORDABLE, typename TYPES>

void inbound<RECORDABLE, TYPES>::sponsed(const sponsee<TYPES>* e) {
  m_sponsee = const_cast<sponsee<TYPES>*>(e);
}

template <typename RECORDABLE, typename TYPES>
void inbound<RECORDABLE, TYPES>::end_request_method() {
  for (auto* ptr = m_requests_to_end.get_next_transaction(); ptr; ptr = m_requests_to_end.get_next_transaction()) {
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;  // TODO: should set according to the target capability
    m_responses_to_begin.notify(*ptr, delay);
    tlm::tlm_phase phase = tlm::END_REQ;
    auto status = this->get_base_port()->nb_transport_bw(*ptr, phase, delay);
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
template <typename RECORDABLE, typename TYPES>
void inbound<RECORDABLE, TYPES>::begin_response_method() {
  for (auto* ptr = m_responses_to_begin.get_next_transaction(); ptr;
       ptr = m_responses_to_begin.get_next_transaction()) {
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;  // TODO: should set according to the target capability
    tlm::tlm_phase phase = tlm::BEGIN_RESP;
    next_trigger(m_responses_to_begin.get_event());
    // TODO: process the request and start response
    m_sponsee->fulfill(*ptr);
    auto status = this->get_base_port()->nb_transport_bw(*ptr, phase, delay);
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
