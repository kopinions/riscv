#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <libremote-port/remote-port-tlm-pci-ep.h>
#include <soc.hpp>

class tlm_t : public sc_core::sc_module {
  SC_HAS_PROCESS(tlm_t);
  sc_signal<bool> rst;
  soc_t<2> m_soc;

 public:
  remoteport_tlm_pci_ep rp_pci_ep;
  tlm_t(const sc_module_name& nm, const char* sk_descr, sc_time quantum)
      : sc_module(nm), rp_pci_ep("rp_pci_ep", 0, 1, 4, sk_descr), m_soc("soc_device") {
    rp_pci_ep.rst(rst);
    rp_pci_ep.bind(m_soc);
  }

 private:
};

void usage(void) { cout << "tlm socket-path sync-quantum-ns" << endl; }

int sc_main(int argc, char* argv[]) {
  tlm_t* top;
  uint64_t sync_quantum;
  sc_trace_file* trace_fp = NULL;

  if (argc < 3) {
    sync_quantum = 10000;
  } else {
    sync_quantum = strtoull(argv[2], NULL, 10);
  }

  sc_set_time_resolution(1, SC_PS);

  top = new tlm_t("top", argv[1], sc_time((double)sync_quantum, SC_NS));

  if (argc < 3) {
    sc_start(1, SC_PS);
    sc_stop();
    usage();
    exit(EXIT_FAILURE);
  }

  trace_fp = sc_create_vcd_trace_file(argv[0]);
  // trace(trace_fp, *top, top->name());

  sc_start();
  if (trace_fp) {
    sc_close_vcd_trace_file(trace_fp);
  }
  return 0;
}
