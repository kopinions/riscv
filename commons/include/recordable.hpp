#pragma once
#include <tlm>

#include "recorder.hpp"

class recordable {
 public:
  virtual bool enabled() const = 0;
  virtual ~recordable() = default;
};

template <unsigned int BUSWIDTH = 32, typename TYPES = tlm::tlm_base_protocol_types, int N = 1,
          sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND>
class inbound : public tlm::tlm_target_socket<BUSWIDTH, TYPES, N, POL> {
 public:
  [[nodiscard]] bool enabled() const;
  virtual ~inbound();
};

template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
bool inbound<BUSWIDTH, TYPES, N, POL>::enabled() const {
  return true;
}
template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
inbound<BUSWIDTH, TYPES, N, POL>::~inbound() {}

template <unsigned int BUSWIDTH = 32, typename TYPES = tlm::tlm_base_protocol_types, int N = 1,
          sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND>
class outbound : public tlm::tlm_initiator_socket<BUSWIDTH, TYPES, N, POL> {
 public:
  using fw_interface_type = tlm::tlm_fw_transport_if<TYPES>;
  using bw_interface_type = tlm::tlm_bw_transport_if<TYPES>;

  using port_type = sc_core::sc_port<fw_interface_type, N, POL>;
  using export_type = sc_core::sc_export<bw_interface_type>;

  using base_target_socket_type = tlm::tlm_base_target_socket_b<BUSWIDTH, fw_interface_type, bw_interface_type>;
  using base_type = tlm::tlm_base_initiator_socket_b<BUSWIDTH, fw_interface_type, bw_interface_type>;

  outbound();
  outbound(const char*);
  [[nodiscard]] bool enabled() const;
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
void outbound<BUSWIDTH, TYPES, N, POL>::bind(bw_interface_type& ifs) {
  this->get_base_export()(ifs);
}
template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
bool outbound<BUSWIDTH, TYPES, N, POL>::enabled() const {
  return true;
}

template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
outbound<BUSWIDTH, TYPES, N, POL>::outbound()
    : tlm::tlm_initiator_socket<BUSWIDTH, TYPES, N, POL>{sc_core::sc_gen_unique_name("outbound_initiator")},
      m_fw(sc_core::sc_gen_unique_name("fw")),
      m_bw(sc_core::sc_gen_unique_name("bw")),
      m_recorder(sc_core::sc_gen_unique_name("recorder"), m_fw, m_bw) {}

#include <sstream>
static inline std::string gen_name(const char* first, const char* second) {
  std::stringstream ss;
  ss << first << "_" << second;

  return ss.str();
}

template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
outbound<BUSWIDTH, TYPES, N, POL>::outbound(const char* name)
    : tlm::tlm_initiator_socket<BUSWIDTH, TYPES, N, POL>{name},
      m_fw(sc_core::sc_gen_unique_name("fw")),
      m_bw(sc_core::sc_gen_unique_name("bw")),
      m_recorder(gen_name(name, "tx").c_str(), m_fw, m_bw) {}
