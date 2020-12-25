#pragma once
class recordable_payload {
 public:
  explicit recordable_payload(tlm::tlm_generic_payload const& o) {}

  sc_dt::uint64 address{0};
  tlm::tlm_command command{tlm::TLM_IGNORE_COMMAND};
  unsigned char* data{nullptr};
  unsigned int data_length{0};
  tlm::tlm_response_status response_status{tlm::TLM_INCOMPLETE_RESPONSE};
  bool dmi_allowed{false};
  unsigned char* byte_enable{nullptr};
  unsigned int byte_enable_length{0};
  unsigned int streaming_width{0};
  tlm::tlm_gp_option gp_option{tlm::TLM_MIN_PAYLOAD};
  uintptr_t uid;
};