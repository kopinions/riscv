#pragma once

#include <functional>
#include <sstream>
#include <tlm>

#include "peers.hpp"

template <typename RECORDABLE, typename TYPES = tlm::tlm_base_protocol_types>
class outbound : public RECORDABLE {
 public:
  using transaction_type = typename TYPES::tlm_payload_type;
  using phase_type = typename TYPES::tlm_phase_type;
  using sync_enum_type = tlm::tlm_sync_enum;
  using fw_interface_type = tlm::tlm_fw_transport_if<TYPES>;
  using bw_interface_type = tlm::tlm_bw_transport_if<TYPES>;

 public:
  outbound() : outbound(sc_core::sc_gen_unique_name("initiator_mixin_socket")) {}

  explicit outbound(const sc_core::sc_module_name& name) : RECORDABLE(name), bw_if(this->name()) {
    this->m_export.bind(bw_if);
    bw_if.set_transport_function([this](transaction_type& tx, phase_type& p, sc_core::sc_time&) -> tlm::tlm_sync_enum {
      tlm::tlm_sync_enum status = tlm::TLM_ACCEPTED;
      if (auto stored_tx = m_bw_txs.find(&tx); stored_tx != m_bw_txs.end()) {
        switch (p) {
          case tlm::END_REQ: {
            m_enable_next_request_event.notify(SC_ZERO_TIME);
            stored_tx->second = tlm::END_REQ;
            status = tlm::TLM_ACCEPTED;
            break;
          }
          case tlm::BEGIN_RESP: {
            m_send_end_rsp_PEQ.notify(tx, SC_ZERO_TIME);
            std::visit(
                [this](auto t) {
                  if (std::is_same_v<decltype(t), tlm::tlm_sync_enum>) {
                    // 3 phase transaction, BEGIN_RESP without END_REQ
                    m_enable_next_request_event.notify(SC_ZERO_TIME);
                  }
                },
                stored_tx->second);

            m_bw_txs.erase(&tx);
            status = tlm::TLM_ACCEPTED;
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
    });
  }

  sponsor_holder<TYPES> transport(transaction_type& payload) {
    const sponsor_holder<TYPES>& holder = sponsor_holder<TYPES>{};
    phase_type phase = tlm::BEGIN_REQ;
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
    auto status = this->get_base_port()->nb_transport_fw(payload, phase, delay);
    sc_core::sc_time m_end_rsp_delay = sc_core::sc_time(10, sc_core::SC_NS);

    switch (status) {
      case tlm::TLM_COMPLETED: {
        wait(delay + m_end_rsp_delay);
        EXPECT_THAT(std::string(reinterpret_cast<char*>(payload.get_data_ptr())), testing::Eq("done"));
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
    if (holder.m_sponsor) {
      holder.m_sponsor->fulfilled(payload);
    }
    return holder;
  };

 private:
  using stype = std::variant<tlm::tlm_sync_enum, tlm::tlm_phase_enum>;

  typedef std::map<tlm::tlm_generic_payload*, stype> waiting_bw_path_map;
  waiting_bw_path_map m_bw_txs;
  tlm_utils::peq_with_get<tlm::tlm_generic_payload> m_send_end_rsp_PEQ{"end_peq"};
  sc_core::sc_event m_enable_next_request_event;
  class bw_transport_if : public tlm::tlm_bw_transport_if<TYPES> {
   public:
    using transport_fct = std::function<sync_enum_type(transaction_type&, phase_type&, sc_core::sc_time&)>;
    using invalidate_dmi_fct = std::function<void(sc_dt::uint64, sc_dt::uint64)>;

    explicit bw_transport_if(const std::string& name)
        : m_name(name), m_transport_ptr(0), m_invalidate_direct_mem_ptr(0) {}

    void set_transport_function(transport_fct p) {
      if (m_transport_ptr) {
        std::stringstream s;
        s << m_name << ": non-blocking callback allready registered";
        SC_REPORT_WARNING("/OSCI_TLM-2/simple_socket", s.str().c_str());
      } else {
        m_transport_ptr = p;
      }
    }

    void set_invalidate_direct_mem_function(invalidate_dmi_fct p) {
      if (m_invalidate_direct_mem_ptr) {
        std::stringstream s;
        s << m_name << ": invalidate DMI callback allready registered";
        SC_REPORT_WARNING("/OSCI_TLM-2/simple_socket", s.str().c_str());
      } else {
        m_invalidate_direct_mem_ptr = p;
      }
    }

    sync_enum_type nb_transport_bw(transaction_type& trans, phase_type& phase, sc_core::sc_time& t) {
      if (m_transport_ptr) return m_transport_ptr(trans, phase, t);
      std::stringstream s;
      s << m_name << ": no transport callback registered";
      SC_REPORT_ERROR("/OSCI_TLM-2/initiator_mixin", s.str().c_str());
      return tlm::TLM_ACCEPTED;  ///< unreachable code
    }

    void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range) {
      if (m_invalidate_direct_mem_ptr)  // forward call
        m_invalidate_direct_mem_ptr(start_range, end_range);
    }

   private:
    const std::string m_name;
    transport_fct m_transport_ptr;
    invalidate_dmi_fct m_invalidate_direct_mem_ptr;
  };

 private:
  bw_transport_if bw_if;
};