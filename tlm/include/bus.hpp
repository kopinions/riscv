#ifndef BUS_HPP
#define BUS_HPP

#include <tlm_utils/multi_passthrough_initiator_socket.h>
#include <tlm_utils/multi_passthrough_target_socket.h>

#include <iostream>
#include <systemc>

template <unsigned int BUSWIDTH = 32>
class bus : public sc_core::sc_module {
 public:
  void b_transport(int, tlm::tlm_generic_payload& payload, sc_core::sc_time&) {
    std::cout << "bus receive" << std::endl;
    sc_dt::uint64 masked_address;
    // TODO should able to use address to distinguish the different device
    //    unsigned int target = decode_address(payload.get_address(), masked_address);
    //    payload.set_address(masked_address);

    sc_core::sc_time t = sc_core::SC_ZERO_TIME;
    m_initiators[0]->b_transport(payload, t);
  }
  tlm::tlm_sync_enum nb_transport_fw(int, tlm::tlm_generic_payload&, tlm::tlm_phase&, sc_core::sc_time&) {
    return tlm::TLM_UPDATED;
  }
  bool get_direct_mem_ptr(int, tlm::tlm_generic_payload& txn, tlm::tlm_dmi& dmi) { return false; }
  unsigned int transport_dbg(int, tlm::tlm_generic_payload& txn) { return 0; }
  tlm::tlm_sync_enum nb_transport_bw(int, tlm::tlm_generic_payload&, tlm::tlm_phase&, sc_core::sc_time&) {
    return tlm::TLM_UPDATED;
  }
  void invalidate_direct_mem_ptr(int, unsigned long long int, unsigned long long int) {}

  bus(const sc_core::sc_module_name& name) noexcept
      : sc_core::sc_module{name}, m_targets{"m_targets"}, m_initiators{"m_initiators"} {
    {
      m_targets.register_b_transport(this, &bus::b_transport);
      m_targets.register_nb_transport_fw(this, &bus::nb_transport_fw);
      m_targets.register_get_direct_mem_ptr(this, &bus::get_direct_mem_ptr);
      m_targets.register_transport_dbg(this, &bus::transport_dbg);
      m_initiators.register_nb_transport_bw(this, &bus::nb_transport_bw);
      m_initiators.register_invalidate_direct_mem_ptr(this, &bus::invalidate_direct_mem_ptr);
    }
  }
  tlm_utils::multi_passthrough_target_socket<bus, BUSWIDTH> m_targets;
  tlm_utils::multi_passthrough_initiator_socket<bus, BUSWIDTH> m_initiators;
};
#endif  // BUS_HPP
