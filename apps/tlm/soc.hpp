#ifndef SOC_HPP
#define SOC_HPP
#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <unistd.h>
// clang-format off
#include <soc/pci/core/pci-device-base.h>
// clang-format on
using namespace sc_core;

template <int NUM_USR_IRQ>
class soc_t : public pci_device_base {
  SC_HAS_PROCESS(soc_t);

 public:
  tlm_utils::simple_initiator_socket<soc_t> tlm_m_axib;
  tlm_utils::simple_target_socket<soc_t> tlm_s_axib;
  sc_vector<sc_in<bool>> usr_irq_reqv;
  sc_vector<sc_signal<bool>> irq_r;
  explicit soc_t(sc_core::sc_module_name name)
      : pci_device_base(name, 1, NUM_USR_IRQ),
        tlm_m_axib("tlm_m_axib"),
        tlm_s_axib("tlm_s_axib"),
        usr_irq_reqv("usr-irq-reqv", NUM_USR_IRQ),
        irq_r("irq-r", NUM_USR_IRQ) {
    int i;

    tlm_s_axib.register_b_transport(this, &soc_t::tlm_s_axib_b_transport);

    SC_THREAD(handle_irqv);
    for (i = 0; i < NUM_USR_IRQ; i++) {
      usr_irq_reqv[i](irq_r[i]);
    }
    for (i = 0; i < NUM_USR_IRQ; i++) {
      sensitive << usr_irq_reqv[i];
    }
  };

 protected:
  void config_b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) override {
    std::cout << "config message" << std::endl;
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
  }

  // Extends the PCI device base class forwarding BAR0 traffic
  // onto the m_axib port.
  void bar_b_transport(int bar_nr, tlm::tlm_generic_payload& trans, sc_time& delay) override {
    tlm_m_axib->b_transport(trans, delay);
  };

  void tlm_s_axib_b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
    dma->b_transport(trans, delay);
  };

  // Map usr_irq_reqv onto the PCI Base class.
  //
  // For now, this is a direct mapping on to the PCI base IRQ vector.
  // In the future we may want to implement our own mapping
  // to MSI/MSI-X without relying on QEMU doing that for us.
  void handle_irqv() {
    int i;

    while (true) {
      // Wait for sensitivity on any usr_irqv[]
      wait();

      for (i = 0; i < NUM_USR_IRQ; i++) {
        irq[i] = usr_irq_reqv[i];
      }
    }
  };
};

#endif  // SOC_HPP
