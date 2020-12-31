#pragma once
#include <tlm.h>


template <typename TYPES = tlm::tlm_base_protocol_types>
class sponsor {
 public:
  using tlm_payload_type = typename TYPES::tlm_payload_type;
  virtual void fulfilled(tlm_payload_type&) = 0;
};

template <typename TYPES = tlm::tlm_base_protocol_types>
class sponsee {
 public:
  using tlm_payload_type = typename TYPES::tlm_payload_type;
  virtual void fulfill(tlm_payload_type&) = 0;
};

template <typename TYPES = tlm::tlm_base_protocol_types>
class sponsor_holder {
 public:
  void from(sponsor<TYPES>* sponsor) { m_sponsor = sponsor; };

 private:
  template <typename, typename>
  friend class outbound;
  sponsor<TYPES>* m_sponsor;
};

template <typename TYPES = tlm::tlm_base_protocol_types>
class sponsee_holder {
 public:
  void from(sponsee<TYPES>* sponsee) { m_sponsee = sponsee; };

 private:
  template <typename, typename>
  friend class inbound;
  sponsee<TYPES>* m_sponsee;
};