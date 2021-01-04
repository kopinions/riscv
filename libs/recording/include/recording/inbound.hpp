#pragma once

#include <tlm.h>
#include <tlm_utils/peq_with_get.h>

#include "peers.hpp"
#include "recordable_target_socket.hpp"

template <typename RECORDABLE, typename TYPES = tlm::tlm_base_protocol_types>
class inbound : public RECORDABLE, public sc_core::sc_module {
  SC_HAS_PROCESS(inbound);

 public:
  friend class fw_process;
  inbound();

  explicit inbound(const sc_core::sc_module_name& name);

  void sponsed(const sponsee<TYPES>* e);

 private:
  class fw_process : public tlm::tlm_fw_transport_if<TYPES> {
   public:
    fw_process(inbound<RECORDABLE, TYPES>* i) : m_inbound{i} {}
    tlm::tlm_sync_enum nb_transport_fw(typename TYPES::tlm_payload_type& trans, typename TYPES::tlm_phase_type& phase,
                                       sc_time& t) override {
      auto status = tlm::TLM_ACCEPTED;
      switch (phase) {
        case tlm::BEGIN_REQ: {
          m_inbound->m_requests_to_end.notify(trans, t);
          break;
        }
        case tlm::END_REQ:
        case tlm::BEGIN_RESP: {
          std::cerr << "illegal status" << std::endl;
          break;
        }
        case tlm::END_RESP: {
          m_inbound->m_end_resp_rcvd_event.notify();
          status = tlm::TLM_COMPLETED;
          break;
        }
        default:
          break;
      }
      return status;
    }
    void b_transport(typename TYPES::tlm_payload_type& trans, sc_time& t) override {}
    bool get_direct_mem_ptr(typename TYPES::tlm_payload_type& trans, tlm::tlm_dmi& dmi_data) override { return false; }
    unsigned int transport_dbg(typename TYPES::tlm_payload_type& trans) override { return 0; }

   private:
    inbound<RECORDABLE, TYPES>* m_inbound;
  };

  fw_process m_fw_process;
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