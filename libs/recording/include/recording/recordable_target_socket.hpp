#pragma once
#include "recording/recordable.hpp"

template <unsigned int BUSWIDTH = 32, typename TYPES = tlm::tlm_base_protocol_types, int N = 1,
          sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND>
class recordable_target_socket : public recordable, public tlm::tlm_target_socket<BUSWIDTH, TYPES, N, POL> {
 public:
  [[nodiscard]] bool enabled() const override;
  virtual ~recordable_target_socket();
};

template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
bool recordable_target_socket<BUSWIDTH, TYPES, N, POL>::enabled() const {
  return true;
}
template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
recordable_target_socket<BUSWIDTH, TYPES, N, POL>::~recordable_target_socket() {}
