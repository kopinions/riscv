#pragma once

#include <tlm.h>

#include "peers.hpp"
#include "recordable_target_socket.hpp"

template <typename RECORDABLE, typename TYPES = tlm::tlm_base_protocol_types>
class inbound : public RECORDABLE, public sc_core::sc_module {
  SC_HAS_PROCESS(inbound);

 public:
  inbound();

  explicit inbound(const sc_core::sc_module_name& name);

  void sponsed(const sponsee<TYPES>* e);

 private:
  sponsee<TYPES>* m_sponsee;
  tlm_utils::peq_with_get<tlm::tlm_generic_payload> m_requests_to_end;
  tlm_utils::peq_with_get<tlm::tlm_generic_payload> m_responses_to_begin;
  const sc_time m_accept_delay;
  sc_event m_end_resp_rcvd_event;

 private:
  void end_request_method();
  void begin_response_method();
};
#include "inbound.tpp"