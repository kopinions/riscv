#pragma once

class recordable_data {
 public:
  explicit recordable_data(const tlm::tlm_generic_payload& payload)
      : address(payload.get_address()),
        command(payload.get_command()),
        data_ptr(payload.get_data_ptr()),
        data_length(payload.get_data_length()),
        response_status(payload.get_response_status()),
        dmi_allowed(payload.is_dmi_allowed()),
        byte_enable(payload.get_byte_enable_ptr()),
        byte_enable_length(payload.get_byte_enable_length()),
        streaming_width(payload.get_streaming_width()),
        gp_option(payload.get_gp_option()),
        uid(reinterpret_cast<uintptr_t>(&payload)) {
    // FIXME: this is endianess dependent
    for (size_t i = 0; i < data_length; i++) {
      data_thumbnail += (*data_ptr) << i * 8;
    }
  }

  sc_dt::uint64 address{0};
  tlm::tlm_command command{tlm::TLM_IGNORE_COMMAND};
  unsigned char* data_ptr{nullptr};
  unsigned int data_length{0};
  uint64_t data_thumbnail = 0;
  tlm::tlm_response_status response_status{tlm::TLM_INCOMPLETE_RESPONSE};
  bool dmi_allowed{false};
  unsigned char* byte_enable{nullptr};
  unsigned int byte_enable_length{0};
  unsigned int streaming_width{0};
  tlm::tlm_gp_option gp_option{tlm::TLM_MIN_PAYLOAD};
  uintptr_t uid;
};