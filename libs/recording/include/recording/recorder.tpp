template <typename TYPES>
recorder<TYPES>::recorder(const char* name, sc_port_b<tlm::tlm_fw_transport_if<TYPES>>& fw_port,
                          sc_port_b<tlm::tlm_bw_transport_if<TYPES>>& bw_port, bool recording_enabled, scv_tr_db* tr_db)
    : sc_core::sc_object(name),
      tracing_enabled("enableTracing", recording_enabled),
      timed_enabled("enableTimed", recording_enabled),
      fw_port(fw_port),
      bw_port(bw_port),
      m_db(tr_db),
      m_base_name{::get_parent(sc_core::sc_object::name())},
      m_nb_timed{tr_db} {
  this->add_attribute(tracing_enabled);
  this->add_attribute(timed_enabled);
  if (tracing_enabled.value) {
    nb_fw_transactor.reserve(3);
    nb_bw_transactor.reserve(3);
    auto* nb_fw_stream = new scv_tr_stream((m_base_name + "_nb_fw").c_str(), "TRANSACTOR", m_db);
    nb_fw_transactor[tlm::TLM_READ_COMMAND] =
        new scv_tr_generator<std::string, tlm::tlm_sync_enum>("read", *nb_fw_stream, "tlm_phase", "tlm_sync");
    nb_fw_transactor[tlm::TLM_WRITE_COMMAND] =
        new scv_tr_generator<std::string, tlm::tlm_sync_enum>("write", *nb_fw_stream, "tlm_phase", "tlm_sync");
    nb_fw_transactor[tlm::TLM_IGNORE_COMMAND] =
        new scv_tr_generator<std::string, tlm::tlm_sync_enum>("ignore", *nb_fw_stream, "tlm_phase", "tlm_sync");

    auto* nb_bw_stream = new scv_tr_stream((m_base_name + "_nb_bw").c_str(), "TRANSACTOR", m_db);
    nb_bw_transactor[tlm::TLM_READ_COMMAND] =
        new scv_tr_generator<std::string, tlm::tlm_sync_enum>("read", *nb_bw_stream, "tlm_phase", "tlm_sync");
    nb_bw_transactor[tlm::TLM_WRITE_COMMAND] =
        new scv_tr_generator<std::string, tlm::tlm_sync_enum>("write", *nb_bw_stream, "tlm_phase", "tlm_sync");
    nb_bw_transactor[tlm::TLM_IGNORE_COMMAND] =
        new scv_tr_generator<std::string, tlm::tlm_sync_enum>("ignore", *nb_bw_stream, "tlm_phase", "tlm_sync");
  }
}

template <typename TYPES>
recorder<TYPES>::recorder(sc_port_b<tlm::tlm_fw_transport_if<TYPES>>& fw_port,
                          sc_port_b<tlm::tlm_bw_transport_if<TYPES>>& bw_port, bool recording_enabled, scv_tr_db* tr_db)
    : recorder{sc_core::sc_gen_unique_name("recorder"), fw_port, bw_port, recording_enabled, tr_db} {}

template <typename TYPES>
tlm::tlm_sync_enum recorder<TYPES>::nb_transport_bw(typename TYPES::tlm_payload_type& trans,
                                                    typename TYPES::tlm_phase_type& phase, sc_core::sc_time& t) {
  return tlm::TLM_UPDATED;
}

template <typename TYPES>
tlm::tlm_sync_enum recorder<TYPES>::nb_transport_fw(typename TYPES::tlm_payload_type& payload,
                                                    typename TYPES::tlm_phase_type& phase, sc_core::sc_time& t) {
  if (!enabled()) {
    return fw_port->nb_transport_fw(payload, phase, t);
  }

  scv_tr_handle h = nb_fw_transactor[payload.get_command()]->begin_transaction(std::string{phase.get_name()});
  if (timed_enabled.value) {
    auto* req = singleton<mm>::get().alloc();
    req->acquire();
    (*req) = payload;
    req->parent = h;
    m_nb_timed.record(*req, phase, t);
  }
  auto prev = payload.template get_extension<recording_extension>();
  if (prev == nullptr) {  // we are the first recording this transaction
    prev = new recording_extension(h, this);
    if (payload.has_mm())
      payload.set_auto_extension(prev);
    else
      payload.set_extension(prev);
  } else {
    h.add_relation(relationship_name(PREDECESSOR_SUCCESSOR), prev->tx());
  }
  // update the extension
  if (prev) {
    prev->tx(h);
  }
  h.record_attribute("delay", t.to_string());

  tlm::tlm_sync_enum status = fw_port->nb_transport_fw(payload, phase, t);
  h.record_attribute("trans", recordable_data{payload});
  h.record_attribute("tlm_phase[return_path]", std::string{phase.get_name()});
  h.record_attribute("delay[return_path]", t.to_string());
  // get the extension and free the memory if it was mine
  if (status == tlm::TLM_COMPLETED || (status == tlm::TLM_ACCEPTED && phase == tlm::END_RESP)) {
    payload.get_extension(prev);
    if (prev && prev->created_by() == this) {
      payload.set_extension(static_cast<recording_extension*>(nullptr));
      if (!payload.has_mm()) {
        delete prev;
      }
    }
    /*************************************************************************
     * do the timed notification if req. finished here
     *************************************************************************/
    //    if (timed_enabled.value) {
    //      tlm_recording_payload* req = mm::get().allocate();
    //      req->acquire();
    //      (*req) = payload;
    //      req->parent = h;
    //      nb_timed_peq.notify(*req, (status == tlm::TLM_COMPLETED && phase == tlm::BEGIN_REQ) ? tlm::END_RESP : phase,
    //                          t);
    //    }
  }
  // else if (timed_enabled.value && status == tlm::TLM_UPDATED) {
  //    tlm_recording_payload* req = mm::get().allocate();
  //    req->acquire();
  //    (*req) = trans;
  //    req->parent = h;
  //    nb_timed_peq.notify(*req, phase, delay);
  //  }
  // End the transaction
  nb_fw_transactor[payload.get_command()]->end_transaction(h, status);
  return status;
}

template <typename TYPES>
void recorder<TYPES>::b_transport(typename TYPES::tlm_payload_type& payload, sc_core::sc_time& t) {
  if (!enabled()) {
    fw_port->b_transport(payload, t);
    return;
  }
}

template <typename TYPES>
bool recorder<TYPES>::get_direct_mem_ptr(typename TYPES::tlm_payload_type&, tlm::tlm_dmi& dmi_data) {
  return false;
}

template <typename TYPES>
unsigned int recorder<TYPES>::transport_dbg(typename TYPES::tlm_payload_type&) {
  return 0;
}

template <typename TYPES>
void recorder<TYPES>::invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range) {}

template <typename TYPES>
bool recorder<TYPES>::enabled() const {
  return m_db != nullptr && tracing_enabled.value;
}