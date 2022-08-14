#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
// clang-format off
#include <libremote-port/remote-port-tlm-pci-ep.h>
// clang-format on
#include <tlm_utils/tlm_quantumkeeper.h>

#include "soc.hpp"

class rtl_t : public sc_core::sc_module {
  SC_HAS_PROCESS(tlm_t);
  sc_signal<bool> rst;
  soc_t<1> m_soc;

 public:
  remoteport_tlm_pci_ep rp_pci_ep;
  tlm_utils::simple_initiator_socket<tlm_t> m_initiator;
  tlm_utils::simple_target_socket<tlm_t> m_target;
  rtl_t(const sc_module_name& nm, const char* sk_descr, sc_time quantum)
      : sc_module(nm),
        rp_pci_ep("rp_pci_ep", 0, 1, 1, sk_descr),
        m_soc("soc_device"),
        m_initiator("init"),
        m_target("target") {
    std::cout << "tlm init" << std::endl;
    m_qk.set_global_quantum(quantum);
    rp_pci_ep.rst(rst);
    rp_pci_ep.bind(m_soc);
    m_soc.tlm_m_axib.bind(m_target);
    m_soc.tlm_s_axib.bind(m_initiator);
    SC_THREAD(pull_reset);
    m_target.register_b_transport(this, &tlm_t::dummy);
    std::cout << "tlm init done" << std::endl;
  }

  void pull_reset() {
    std::cout << "reset" << std::endl;
    rst.write(true);
    wait(1, SC_US);
    rst.write(false);
    std::cout << "reset done" << std::endl;
  }

  void dummy(tlm::tlm_generic_payload& trans, sc_time& delay) {
    std::cout << "xxxx" << std::endl;
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
  };

 private:
  tlm_utils::tlm_quantumkeeper m_qk;
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

  //  sc_set_time_resolution(1, SC_PS);

  std::cout << "tlm_t" << std::endl;
  top = new rtl_t("tlm_t", argv[1], sc_time((double)sync_quantum, SC_NS));

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
