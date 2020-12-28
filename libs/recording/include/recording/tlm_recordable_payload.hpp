#pragma once
#include <tlm.h>
#include <scv.h>

template <typename TYPES = tlm::tlm_base_protocol_types>
class tlm_recordable_payload : public TYPES::tlm_payload_type {
 public:
  scv_tr_handle parent;
  uint64 id;
  tlm_recordable_payload& operator=(const typename TYPES::tlm_payload_type& x) {
    id = reinterpret_cast<uintptr_t>(&x);
    this->set_command(x.get_command());
    this->set_address(x.get_address());
    this->set_data_ptr(nullptr);
    this->set_data_length(x.get_data_length());
    this->set_response_status(x.get_response_status());
    this->set_byte_enable_ptr(nullptr);
    this->set_byte_enable_length(x.get_byte_enable_length());
    this->set_streaming_width(x.get_streaming_width());
    return (*this);
  }

  explicit tlm_recordable_payload(tlm::tlm_mm_interface* mm) : TYPES::tlm_payload_type(mm), parent(), id(0) {}
};

template <typename TYPES = tlm::tlm_base_protocol_types>
struct tlm_recordable_protocol_types {
  using tlm_payload_type = tlm_recordable_payload<TYPES>;
  using tlm_phase_type = typename TYPES::tlm_phase_type;
};
