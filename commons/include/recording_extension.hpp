#pragma once

#include <scv.h>
#include <tlm.h>

template <>
class scv_extensions<tlm::tlm_sync_enum> : public scv_enum_base<tlm::tlm_sync_enum> {
 public:
  SCV_ENUM_CTOR(tlm::tlm_sync_enum) {  // NOLINT
    SCV_ENUM(tlm::TLM_ACCEPTED);       // NOLINT
    SCV_ENUM(tlm::TLM_UPDATED);        // NOLINT
    SCV_ENUM(tlm::TLM_COMPLETED);      // NOLINT
  }
};


template <> class scv_extensions<tlm::tlm_phase_enum> : public scv_enum_base<tlm::tlm_phase_enum> {
 public:
  SCV_ENUM_CTOR(tlm::tlm_phase_enum) {//NOLINT
    SCV_ENUM(tlm::UNINITIALIZED_PHASE);//NOLINT
    SCV_ENUM(tlm::BEGIN_REQ);//NOLINT
    SCV_ENUM(tlm::END_REQ);//NOLINT
    SCV_ENUM(tlm::BEGIN_RESP);//NOLINT
    SCV_ENUM(tlm::END_RESP);//NOLINT
  }
};

template <>
class scv_extensions<tlm::tlm_command> : public scv_enum_base<tlm::tlm_command> {
 public:
  SCV_ENUM_CTOR(tlm::tlm_command) {     // NOLINT
    SCV_ENUM(tlm::TLM_READ_COMMAND);    // NOLINT
    SCV_ENUM(tlm::TLM_WRITE_COMMAND);   // NOLINT
    SCV_ENUM(tlm::TLM_IGNORE_COMMAND);  // NOLINT
  }
};

template <>
class scv_extensions<tlm::tlm_response_status> : public scv_enum_base<tlm::tlm_response_status> {
 public:
  SCV_ENUM_CTOR(tlm::tlm_response_status) {         // NOLINT
    SCV_ENUM(tlm::TLM_OK_RESPONSE);                 // NOLINT
    SCV_ENUM(tlm::TLM_INCOMPLETE_RESPONSE);         // NOLINT
    SCV_ENUM(tlm::TLM_GENERIC_ERROR_RESPONSE);      // NOLINT
    SCV_ENUM(tlm::TLM_ADDRESS_ERROR_RESPONSE);      // NOLINT
    SCV_ENUM(tlm::TLM_COMMAND_ERROR_RESPONSE);      // NOLINT
    SCV_ENUM(tlm::TLM_BURST_ERROR_RESPONSE);        // NOLINT
    SCV_ENUM(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);  // NOLINT
  }
};

template <>
class scv_extensions<tlm::tlm_gp_option> : public scv_enum_base<tlm::tlm_gp_option> {
 public:
  SCV_ENUM_CTOR(tlm::tlm_gp_option) {          // NOLINT
    SCV_ENUM(tlm::TLM_MIN_PAYLOAD);            // NOLINT
    SCV_ENUM(tlm::TLM_FULL_PAYLOAD);           // NOLINT
    SCV_ENUM(tlm::TLM_FULL_PAYLOAD_ACCEPTED);  // NOLINT
  }
};

template <>
class scv_extensions<recordable_data> : public scv_extensions_base<recordable_data> {
 public:
  scv_extensions<sc_dt::uint64> address;
  scv_extensions<tlm::tlm_command> command;
  scv_extensions<unsigned char*> data_ptr;
  scv_extensions<unsigned int> data_length;
  scv_extensions<uint64_t> data_thumbnail;
  scv_extensions<tlm::tlm_response_status> response_status;
  scv_extensions<bool> dmi_allowed;
  scv_extensions<unsigned char*> byte_enable;
  scv_extensions<unsigned int> byte_enable_length;
  scv_extensions<unsigned int> streaming_width;
  scv_extensions<tlm::tlm_gp_option> gp_option;
  scv_extensions<uintptr_t> uid;

  SCV_EXTENSIONS_CTOR(recordable_data) {  // NOLINT
    SCV_FIELD(address);
    SCV_FIELD(command);
    SCV_FIELD(data_ptr);
    SCV_FIELD(data_length);
    SCV_FIELD(data_thumbnail);
    SCV_FIELD(response_status);
    SCV_FIELD(dmi_allowed);
    SCV_FIELD(byte_enable);
    SCV_FIELD(byte_enable_length);
    SCV_FIELD(streaming_width);
    SCV_FIELD(gp_option);
    SCV_FIELD(uid);
  }
};

class recording_extension : public tlm::tlm_extension<recording_extension> {
 public:
  recording_extension(scv_tr_handle tx_manager, void* const creator)
      : m_tx_manager{tx_manager}, m_created_by{creator} {}

  [[nodiscard]] tlm_extension_base* clone() const override {
    auto* t = new recording_extension(this->m_tx_manager, this->m_created_by);
    return t;
  }

  void copy_from(const tlm_extension_base& ext) override {}

  ~recording_extension() override = default;

  void* created_by() { return m_created_by; }

  void tx(const scv_tr_handle& tx) { m_tx_manager = tx; }

  [[nodiscard]] const scv_tr_handle& tx() const { return m_tx_manager; }

 private:
  scv_tr_handle m_tx_manager;
  void* m_created_by;
};