#include <remote-port-tlm-memory-master.h>
#include <remote-port-tlm-memory-slave.h>
#include <remote-port-tlm-wires.h>
#include <remote-port-tlm.h>
#include <systemc.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/tlm_quantumkeeper.h>

class pcie_bridge : public remoteport_tlm {
 private:
  remoteport_tlm_memory_slave rp_s_axi_cfg;
  remoteport_tlm_memory_slave rp_s_axi_data;
  remoteport_tlm_wires rp_irq_out;

 public:
  tlm_utils::simple_target_socket<remoteport_tlm_memory_slave> *s_axi[2];
  sc_vector<sc_signal<bool> > irq;

  pcie_bridge(sc_core::sc_module_name name, const char *sk_descr, Iremoteport_tlm_sync *sync = NULL);
};
