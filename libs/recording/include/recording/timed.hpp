#pragma once
#include <tlm.h>
#include <tlm_utils/peq_with_cb_and_phase.h>

#include "recording/relationship.hpp"
#include "recording/tlm_recordable_payload.hpp"

template <typename TYPES>
class timed {
 public:
  using phase_type = typename TYPES::tlm_phase_type;
  using recordable_protocol_types = tlm_recordable_protocol_types<TYPES>;
  using recordable_payload_type = typename recordable_protocol_types::tlm_payload_type;
  timed(scv_tr_db* tr_db) : m_peq{this, &timed::nbtx_cb}, req{3}, res{3} {
    std::string fixed_basename = "timed";
    auto* stream = new scv_tr_stream((fixed_basename + "_nb_req_timed").c_str(), "TRANSACTOR", tr_db);
    req[tlm::tlm_command::TLM_READ_COMMAND] = new scv_tr_generator<>("read", *stream);
    req[tlm::tlm_command::TLM_WRITE_COMMAND] = new scv_tr_generator<>("write", *stream);
    req[tlm::tlm_command::TLM_IGNORE_COMMAND] = new scv_tr_generator<>("ignore", *stream);

    auto* res_stream = new scv_tr_stream((fixed_basename + "_nb_resp_timed").c_str(), "TRANSACTOR", tr_db);
    res[tlm::tlm_command::TLM_READ_COMMAND] = new scv_tr_generator<>("read", *res_stream);
    res[tlm::tlm_command::TLM_WRITE_COMMAND] = new scv_tr_generator<>("write", *res_stream);
    res[tlm::tlm_command::TLM_IGNORE_COMMAND] = new scv_tr_generator<>("ignore", *res_stream);
  }

  void record(recordable_payload_type&, phase_type&, sc_core::sc_time&);

 private:
  void nbtx_cb(recordable_payload_type& payload, const phase_type& phase);
  std::vector<scv_tr_generator<>*> req;
  std::vector<scv_tr_generator<>*> res;
  tlm_utils::peq_with_cb_and_phase<timed, recordable_protocol_types> m_peq;
  map<uint64, scv_tr_handle> nbtx_req_handle_map;
  map<uint64, scv_tr_handle> nbtx_last_req_handle_map;
};

template <typename TYPES>
void timed<TYPES>::nbtx_cb(recordable_payload_type& payload, const phase_type& phase) {
  scv_tr_handle h;
  std::map<uint64, scv_tr_handle>::iterator it;

  recordable_data tgd(payload);
  switch (phase) {
    case tlm::BEGIN_REQ:
      h = req[payload.get_command()]->begin_transaction();
      h.record_attribute("trans", tgd);
      h.add_relation(relationship_name(PARENT_CHILD), payload.parent);
      nbtx_req_handle_map[payload.id] = h;
      break;
    case tlm::END_REQ:
      it = nbtx_req_handle_map.find(payload.id);
      sc_assert(it != nbtx_req_handle_map.end());
      h = it->second;
      nbtx_req_handle_map.erase(it);
      h.end_transaction();
      nbtx_last_req_handle_map[payload.id] = h;
      break;
    case tlm::BEGIN_RESP:
      it = nbtx_req_handle_map.find(payload.id);
      if (it != nbtx_req_handle_map.end()) {
        h = it->second;
        nbtx_req_handle_map.erase(it);
        h.end_transaction();
        nbtx_last_req_handle_map[payload.id] = h;
      }
      h = res[payload.get_command()]->begin_transaction();
      h.record_attribute("trans", tgd);
      h.add_relation(relationship_name(PARENT_CHILD), payload.parent);
      nbtx_req_handle_map[payload.id] = h;
      it = nbtx_last_req_handle_map.find(payload.id);
      if (it != nbtx_last_req_handle_map.end()) {
        scv_tr_handle pred = it->second;
        nbtx_last_req_handle_map.erase(it);
        h.add_relation(relationship_name(PREDECESSOR_SUCCESSOR), pred);
      }
      break;
    case tlm::END_RESP:
      it = nbtx_req_handle_map.find(payload.id);
      if (it != nbtx_req_handle_map.end()) {
        h = it->second;
        nbtx_req_handle_map.erase(it);
        h.end_transaction();
      }
      break;
    default:
      // sc_assert(!"phase not supported!");
      break;
  }
  payload.release();
  return;
}
template <typename TYPES>
void timed<TYPES>::record(recordable_payload_type& payload, phase_type& phase, sc_time& delay) {
  m_peq.notify(payload, phase, delay);
}
