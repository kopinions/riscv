#include "soc.hpp"
template <int NUM_USR_IRQ>
void soc_t<NUM_USR_IRQ>::bar_b_transport(int bar_nr, tlm::tlm_generic_payload& trans, sc_time& delay) {
  tlm_m_axib->b_transport(trans, delay);
}

template <int NUM_USR_IRQ>
void soc_t<NUM_USR_IRQ>::tlm_s_axib_b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
  dma->b_transport(trans, delay);
}

template <int NUM_USR_IRQ>
void soc_t<NUM_USR_IRQ>::handle_irqv(void) {
  int i;

  while (true) {
    // Wait for sensitivity on any usr_irqv[]
    wait();

    for (i = 0; i < NUM_USR_IRQ; i++) {
      irq[i] = usr_irq_reqv[i];
    }
  }
}
template <int NUM_USR_IRQ>
soc_t<NUM_USR_IRQ>::soc_t(sc_core::sc_module_name name) {}
