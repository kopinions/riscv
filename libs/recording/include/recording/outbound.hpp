#pragma once

#include <functional>
#include <sstream>
#include <tlm>

#include "peers.hpp"

template <typename RECORDABLE, typename TYPES = tlm::tlm_base_protocol_types>
class outbound : public RECORDABLE, public sc_core::sc_module {
 public:
  SC_HAS_PROCESS(outbound);
  using transaction_type = typename TYPES::tlm_payload_type;
  using phase_type = typename TYPES::tlm_phase_type;
  using sync_enum_type = tlm::tlm_sync_enum;
  using fw_interface_type = tlm::tlm_fw_transport_if<TYPES>;
  using bw_interface_type = tlm::tlm_bw_transport_if<TYPES>;

 public:
  outbound() : outbound(sc_core::sc_gen_unique_name("outbound")) {}

  explicit outbound(const sc_core::sc_module_name& name)
      : sc_core::sc_module(name), RECORDABLE(name), bw_if(sc_core::sc_module::name(), this) {
    this->m_export.bind(bw_if);
    SC_METHOD(send_end_rsp_method);
    sensitive << m_send_end_rsp_PEQ.get_event();
    dont_initialize();
  }

  void send_end_rsp_method() {
    for (auto* ptr = m_send_end_rsp_PEQ.get_next_transaction(); ptr != nullptr;
         ptr = m_send_end_rsp_PEQ.get_next_transaction()) {
      tlm::tlm_phase phase = tlm::END_RESP;  // set the appropriate phase
      sc_time delay = SC_ZERO_TIME;

      // call begin response and then decode return status
      tlm::tlm_sync_enum return_value = this->get_base_port()->nb_transport_fw(*ptr, phase, delay);

      switch (return_value) {
        case tlm::TLM_COMPLETED: {
          m_sponsor->fulfilled(*ptr);
          break;
        }

        case tlm::TLM_ACCEPTED:
        case tlm::TLM_UPDATED: {
          std::cerr << "error updated" << std::endl;
          break;
        }

        default: {
          std::cerr << "error status" << std::endl;
          break;
        }
      }  // end switch
    }    // end while

    return;
  };

  void from(sponsor<TYPES>* s) { m_sponsor = s; }

  void transport(transaction_type& payload) {
    phase_type phase = tlm::BEGIN_REQ;
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
    auto status = this->get_base_port()->nb_transport_fw(payload, phase, delay);
    sc_core::sc_time m_end_rsp_delay = sc_core::sc_time(10, sc_core::SC_NS);

    switch (status) {
      case tlm::TLM_COMPLETED: {
        wait(delay + m_end_rsp_delay);
        m_sponsor->fulfilled(payload);
        break;
      }
      case tlm::TLM_UPDATED: {
        if (phase == tlm::END_REQ) {
          auto p = std::make_pair(&payload, tlm::END_REQ);
          m_bw_txs.insert(p);
          wait(delay);  // wait annotated time
        } else if (phase == tlm::BEGIN_RESP) {
          wait(delay);
          m_send_end_rsp_PEQ.notify(payload, delay);
        } else {
          std::cerr << "error status";
        }
        break;
      }
      case tlm::TLM_ACCEPTED: {
        auto p = std::make_pair(&payload, tlm::TLM_ACCEPTED);
        m_bw_txs.insert(p);
        // TODO refactor needed to wait the event, change nb_transport_bw to peq
        //        wait(m_enable_next_request_event);
      }
    }
  };

 private:
  using stype = std::variant<tlm::tlm_sync_enum, tlm::tlm_phase_enum>;

  typedef std::map<tlm::tlm_generic_payload*, stype> waiting_bw_path_map;
  waiting_bw_path_map m_bw_txs;
  tlm_utils::peq_with_get<tlm::tlm_generic_payload> m_send_end_rsp_PEQ{"end_peq"};
  sc_core::sc_event m_enable_next_request_event;
  sponsor<TYPES>* m_sponsor;

  class bw_transport_if : public tlm::tlm_bw_transport_if<TYPES> {
   public:
    using transport_fct = std::function<sync_enum_type(transaction_type&, phase_type&, sc_core::sc_time&)>;
    using invalidate_dmi_fct = std::function<void(sc_dt::uint64, sc_dt::uint64)>;

    explicit bw_transport_if(const std::string& name, outbound<RECORDABLE, TYPES>* ob)
        : m_name(name), m_transport_ptr(0), m_invalidate_direct_mem_ptr(0), m_outbound{ob} {}

    sync_enum_type nb_transport_bw(transaction_type& tx, phase_type& phase, sc_core::sc_time& t) {
      tlm::tlm_sync_enum status = tlm::TLM_ACCEPTED;
      if (auto stored_tx = m_outbound->m_bw_txs.find(&tx); stored_tx != m_outbound->m_bw_txs.end()) {
        switch (phase) {
          case tlm::END_REQ: {
            m_outbound->m_enable_next_request_event.notify(SC_ZERO_TIME);
            stored_tx->second = tlm::END_REQ;
            status = tlm::TLM_ACCEPTED;
            break;
          }
          case tlm::BEGIN_RESP: {
            m_outbound->m_send_end_rsp_PEQ.notify(tx, SC_ZERO_TIME);
            std::visit(
                [this](auto t) {
                  if (std::is_same_v<decltype(t), tlm::tlm_sync_enum>) {
                    // 3 phase transaction, BEGIN_RESP without END_REQ
                    m_outbound->m_enable_next_request_event.notify(SC_ZERO_TIME);
                  }
                },
                stored_tx->second);

            m_outbound->m_bw_txs.erase(&tx);
            status = tlm::TLM_COMPLETED;
            break;
          }
          case tlm::BEGIN_REQ:
          case tlm::END_RESP: {
            std::cerr << "error ";
            break;
          }
          default: {
            std::cerr << "Unknown phase on the backward path";
            break;
          }
        }
      } else {
        std::cerr << "error no transaction found";
        status = tlm::TLM_ACCEPTED;
      }

      return status;
    }

    void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range) {
      if (m_invalidate_direct_mem_ptr)  // forward call
        m_invalidate_direct_mem_ptr(start_range, end_range);
    }

   private:
    const std::string m_name;
    transport_fct m_transport_ptr;
    outbound<RECORDABLE, TYPES>* m_outbound;
    invalidate_dmi_fct m_invalidate_direct_mem_ptr;
  };

 private:
  bw_transport_if bw_if;
};