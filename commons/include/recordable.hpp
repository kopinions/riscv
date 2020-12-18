#pragma once
#include <tlm>

#include "recorder.hpp"

class recordable {
 public:
  virtual ~recordable() = 0;
};

template <unsigned int BUSWIDTH = 32, typename TYPES = tlm::tlm_base_protocol_types, int N = 1,
          sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND>
class inbound : public recordable, public tlm::tlm_target_socket<BUSWIDTH, TYPES, N, POL> {};

template <unsigned int BUSWIDTH = 32, typename TYPES = tlm::tlm_base_protocol_types, int N = 1,
          sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND>
class outbound : public recordable, public tlm::tlm_initiator_socket<BUSWIDTH, TYPES, N, POL> {
 public:
  using fw_interface_type = tlm::tlm_fw_transport_if<TYPES>;
  using bw_interface_type = tlm::tlm_bw_transport_if<TYPES>;

  using port_type = sc_core::sc_port<fw_interface_type, N, POL>;
  using export_type = sc_core::sc_export<bw_interface_type>;

  using base_target_socket_type = tlm::tlm_base_target_socket_b<BUSWIDTH, fw_interface_type, bw_interface_type>;
  using base_type = tlm::tlm_base_initiator_socket_b<BUSWIDTH, fw_interface_type, bw_interface_type>;

  void bind(base_target_socket_type& tgt) override;
  void bind(base_type& bt) override;
  void bind(bw_interface_type& ifs) override;
  [[nodiscard]] const char* kind() const override;
  ~outbound() override;

 private:
  sc_core::sc_port<fw_interface_type, N, POL> m_fw;
  sc_core::sc_port<bw_interface_type, N, POL> m_bw;
  recorder<TYPES> m_recorder;
};

template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
const char* outbound<BUSWIDTH, TYPES, N, POL>::kind() const {
  return "outbound_recordable";
}
template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
outbound<BUSWIDTH, TYPES, N, POL>::~outbound() {}

//
// Bind initiator socket to target socket
// - Binds the port of the initiator socket to the export of the target
//   socket
// - Binds the port of the target socket to the export of the initiator
//   socket
template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
void outbound<BUSWIDTH, TYPES, N, POL>::bind(base_target_socket_type& tgt) {
  // this -> recorder -> target

  // bind m_recorder as FW interface to this.port
  this->get_base_port()(m_recorder);
  // bind target interface to recorder.port
  m_fw(tgt.get_base_interface());

  // bind m_recorder as BW interface to target.port
  tgt.get_base_port()(m_recorder);
  // bind this as BW interface recorder.export
  m_bw(this->get_base_interface());
}

// Bind initiator socket to initiator socket (hierarchical bind)
// - Binds both the export and the port
template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
void outbound<BUSWIDTH, TYPES, N, POL>::bind(base_type& bt) {
  // initiator -> recorder -> initiator2
  
  // bind m_recorder as interface to port
  this->get_base_port()(m_recorder);
  // bind initiator.port to bt.port
  m_fw(bt.get_base_port());

  // bind m_recorder as interface to export
  this->get_base_export()(m_recorder);
  // bind initiator.export to bt.export
  m_bw(bt.get_base_export());
}

// Bind interface to socket
// - Binds the interface to the export of this socket
template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
void outbound<BUSWIDTH, TYPES, N, POL>::bind(bw_interface_type& ifs) {
  this->get_base_export()(ifs);
}
