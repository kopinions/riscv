#pragma once
#include "recording/recordable.hpp"

template <unsigned int BUSWIDTH = 32, typename TYPES = tlm::tlm_base_protocol_types, int N = 1,
          sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND>
class recordable_target_socket : public recordable, public tlm::tlm_target_socket<BUSWIDTH, TYPES, N, POL> {
 public:
  using fw_interface_type = tlm::tlm_fw_transport_if<TYPES>;
  using bw_interface_type = tlm::tlm_bw_transport_if<TYPES>;

  using export_type = sc_core::sc_export<fw_interface_type>;
  using port_type = sc_core::sc_export<bw_interface_type>;

  using base_type = tlm::tlm_base_target_socket_b<BUSWIDTH, fw_interface_type, bw_interface_type>;
  using base_initiator_socket_type = tlm::tlm_base_initiator_socket_b<BUSWIDTH, fw_interface_type, bw_interface_type>;

  [[nodiscard]] bool enabled() const override;
  recordable_target_socket();
  explicit recordable_target_socket(const char* name);
  virtual ~recordable_target_socket();

  void bind(base_initiator_socket_type& s) override;
  void bind(base_type& s) override;
  void bind(fw_interface_type& ifs) override;

  bw_interface_type* operator->();

 private:
  sc_core::sc_port<fw_interface_type> m_fw_port;
  recorder<TYPES> m_recorder;
};

template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
bool recordable_target_socket<BUSWIDTH, TYPES, N, POL>::enabled() const {
  return true;
}

template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
recordable_target_socket<BUSWIDTH, TYPES, N, POL>::~recordable_target_socket() {}

template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
void recordable_target_socket<BUSWIDTH, TYPES, N, POL>::bind(base_initiator_socket_type& s) {
  // initiator.port -> target.export
  s.get_base_port()(m_recorder);
  // target.port -> initiator.export
  this->get_base_port()(s.get_base_interface());
}

template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
void recordable_target_socket<BUSWIDTH, TYPES, N, POL>::bind(base_type& s) {
  // nb_transport_bw recorded by recorder
  s.get_base_port()(m_recorder);
  /* handled by bind(fw_interface_type)
    virtual sc_core::sc_export<FW_IF> &       get_base_export()
    { return *this; }

    class sc_export {
    public:
      void operator () ( IF& interface_ )
      {
          this->bind(interface_);
      }
    }
   */
  this->get_base_export()(s.get_base_export());
}

template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
void recordable_target_socket<BUSWIDTH, TYPES, N, POL>::bind(fw_interface_type& ifs) {
  export_type* exp = &this->get_base_export();
  if (this == exp) {
    export_type::bind(ifs);  // call none virtual bind method
    // this->bind(interface)
    m_fw_port(ifs);
  } else {
    exp->bind(ifs);
  }
}

template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
recordable_target_socket<BUSWIDTH, TYPES, N, POL>::recordable_target_socket(const char* name)
    : m_recorder{"tx", m_fw_port, this->get_base_port()} {}

template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
recordable_target_socket<BUSWIDTH, TYPES, N, POL>::recordable_target_socket()
    : recordable_target_socket("recordable_target") {}

template <unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL>
typename recordable_target_socket<BUSWIDTH, TYPES, N, POL>::bw_interface_type*
recordable_target_socket<BUSWIDTH, TYPES, N, POL>::operator->() {
  return m_recorder;
}
