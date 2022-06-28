#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "pcie_bridge.hpp"

#include <inttypes.h>

#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

using namespace sc_core;
using namespace std;

#include <sys/types.h>

pcie_bridge::pcie_bridge(sc_module_name name, const char *sk_descr, Iremoteport_tlm_sync *sync)
    : remoteport_tlm(name, -1, sk_descr, sync),
      rp_s_axi_cfg("rp_s_axi_cfg"),
      rp_s_axi_data("rp_s_axi_data"),
      rp_irq_out("irq_out", 0, 4),
      ps2pl_irq("ps2pl_irq", rp_irq_out.wires_out.size()) {
  int i;

  s_axi[0] = &rp_s_axi_cfg.sk;

  s_axi[1] = &rp_s_axi_data.sk;

  /* PS to PL Interrupt signals.  */
  for (i = 0; i < 4; i++) {
    rp_irq_out.wires_out[i](irq[i]);
  }

  register_dev(0, &rp_s_axi_cfg);
  register_dev(1, &rp_s_axi_data);
  register_dev(2, &rp_irq_out);
}
