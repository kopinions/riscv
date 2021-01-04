#pragma once
#include "recording/recordable.hpp"
#include "recording/recorder.hpp"

template <unsigned int BUSWIDTH = 32, typename TYPES = tlm::tlm_base_protocol_types, int N = 1,
          sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND>
class recordable_initiator_socket : public recordable, public tlm::tlm_initiator_socket<BUSWIDTH, TYPES, N, POL> {
 public:
  using fw_interface_type = tlm::tlm_fw_transport_if<TYPES>;
  using bw_interface_type = tlm::tlm_bw_transport_if<TYPES>;

  using port_type = sc_core::sc_port<fw_interface_type, N, POL>;
  using export_type = sc_core::sc_export<bw_interface_type>;

  using base_target_socket_type = tlm::tlm_base_target_socket_b<BUSWIDTH, fw_interface_type, bw_interface_type>;
  using base_type = tlm::tlm_base_initiator_socket_b<BUSWIDTH, fw_interface_type, bw_interface_type>;

  recordable_initiator_socket();
  recordable_initiator_socket(const char*);
  [[nodiscard]] bool enabled() const override;
  void bind(base_target_socket_type& tgt) override;
  void bind(base_type& bt) override;
  void bind(bw_interface_type& ifs) override;
  [[nodiscard]] const char* kind() const override;
  ~recordable_initiator_socket() override;

 private:
  sc_core::sc_port<fw_interface_type, N, POL> m_fw;
  sc_core::sc_port<bw_interface_type, N, POL> m_bw;
  recorder<TYPES> m_recorder;
  static std::string gen_name(const char* first, const char* second) {
    std::stringstream ss;
    ss << first << "_" << second;
    return ss.str();
  }
};

template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
const char* recordable_initiator_socket<BUSWIDTH, TYPES, N, POL>::kind() const {
  return "outbound_recordable";
}
template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
recordable_initiator_socket<BUSWIDTH, TYPES, N, POL>::~recordable_initiator_socket() {}

//
// Bind initiator socket to target socket
// - Binds the port of the initiator socket to the export of the target
//   socket
// - Binds the port of the target socket to the export of the initiator
//   socket
template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
void recordable_initiator_socket<BUSWIDTH, TYPES, N, POL>::bind(base_target_socket_type& tgt) {
  // this -> recorder -> target

  // bind m_recorder as FW interface to this.port
  sc_port_b<fw_interface_type>& b = this->get_base_port();
  b(m_recorder);
  // bind target interface to recorder.port
  fw_interface_type& anIf = tgt.get_base_interface();
  m_fw(anIf);

  // bind m_recorder as BW interface to target.port
  tgt.get_base_port()(m_recorder);
  // bind this as BW interface recorder.export
  m_bw(this->get_base_interface());
}

// Bind initiator socket to initiator socket (hierarchical bind)
// - Binds both the export and the port
template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
void recordable_initiator_socket<BUSWIDTH, TYPES, N, POL>::bind(base_type& bt) {
  // initiator -> recorder -> initiator2

  // bind initiator.port to recorder.port
  this->get_base_port()(m_recorder);
  // bind recorder.port to bt.port
  m_fw(bt.get_base_port());

  // bind initiator.export to recorder.export
  this->get_base_export()(m_recorder);
  // bind recorder.export to bt.export
  m_bw(bt.get_base_export());
}

// Bind interface to socket
// - Binds the interface to the export of this socket
template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
void recordable_initiator_socket<BUSWIDTH, TYPES, N, POL>::bind(bw_interface_type& ifs) {
  this->get_base_export()(ifs);
}
template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
bool recordable_initiator_socket<BUSWIDTH, TYPES, N, POL>::enabled() const {
  return true;
}

template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
recordable_initiator_socket<BUSWIDTH, TYPES, N, POL>::recordable_initiator_socket()
    : tlm::tlm_initiator_socket<BUSWIDTH, TYPES, N, POL>{sc_core::sc_gen_unique_name("recordable_initiator")},
      m_fw(sc_core::sc_gen_unique_name("fw")),
      m_bw(sc_core::sc_gen_unique_name("bw")),
      m_recorder{sc_core::sc_gen_unique_name(this->name()), m_fw, m_bw} {}

template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
recordable_initiator_socket<BUSWIDTH, TYPES, N, POL>::recordable_initiator_socket(const char* name)
    : tlm::tlm_initiator_socket<BUSWIDTH, TYPES, N, POL>{name},
      m_fw(sc_core::sc_gen_unique_name("fw")),
      m_bw(sc_core::sc_gen_unique_name("bw")),
      m_recorder{sc_core::sc_gen_unique_name("tx"), m_fw, m_bw} {}
