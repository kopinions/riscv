#ifndef TLM2AXI4_HPP
#define TLM2AXI4_HPP
#include <tlm_utils/simple_target_socket.h>

#include <systemc>
template <unsigned int ADDR_WIDTH, unsigned int DATA_WIDTH = 32>
class tlm2axi4 : public sc_core::sc_module {
 public:
  SC_HAS_PROCESS(tlm2axi4);
  tlm_utils::simple_target_socket<tlm2axi4<DATA_WIDTH>> m_target;
  using address_type = sc_dt::sc_bv<ADDR_WIDTH>;
  using data_type = sc_dt::sc_bv<DATA_WIDTH>;
  sc_core::sc_in<sc_dt::sc_bv<ADDR_WIDTH>> m_waddr;
  sc_core::sc_out<sc_dt::sc_bv<DATA_WIDTH>> m_wdata;
  sc_core::sc_out<sc_dt::sc_bv<ADDR_WIDTH>> m_raddr;
  sc_core::sc_in<sc_dt::sc_bv<DATA_WIDTH>> m_rdata;
  sc_core::sc_in<bool> m_clk;
  sc_core::sc_in<bool> m_resetn;

  tlm2axi4(const sc_core::sc_module_name& name) : sc_module(name) {
    m_target.register_b_transport(this, &tlm2axi4<ADDR_WIDTH, DATA_WIDTH>::b_transport);
    SC_THREAD(read_address_phase)
    SC_THREAD(read_response_phase)
    SC_THREAD(write_address_phase)
    SC_THREAD(write_data_phase)
    SC_THREAD(write_response_phase)
  }

 private:
  class transaction {
   public:
    transaction(tlm::tlm_generic_payload& payload) : m_payload{payload} {}

    sc_core::sc_event& done() { return m_done; };
    tlm::tlm_generic_payload& payload() { return m_payload; }

   private:
    tlm::tlm_generic_payload& m_payload;
    sc_core::sc_event m_done;
  };

  sc_core::sc_fifo<transaction*> m_r_transaction_fifo;
  sc_core::sc_fifo<transaction*> m_r_response;
  sc_core::sc_fifo<transaction*> m_w_transaction_fifo;
  sc_core::sc_fifo<transaction*> m_w_data_fifo;
  sc_core::sc_fifo<transaction*> m_w_response;

  sc_core::sc_mutex m_mutex;
  void b_transport(tlm::tlm_generic_payload& payload, sc_core::sc_time&) {
    m_mutex.lock();

    transaction tx{payload};
    if (payload.is_read()) {
      m_r_transaction_fifo.write(&tx);
    } else {
      m_w_transaction_fifo.write(&tx);
    }
    std::cout << "wait" << std::endl;
    wait(tx.done());
    m_mutex.unlock();
  }

  [[noreturn]] void read_address_phase() {
    while (true) {
      transaction* tx = m_r_transaction_fifo.read();
      std::cout << "read_address_phase" << std::endl;
      m_r_response.write(tx);
      std::cout << "read_address_write" << std::endl;
    }
  }

  [[noreturn]] void write_address_phase() {
    while (true) {
      transaction* tx = m_w_transaction_fifo.read();
      m_w_data_fifo.write(tx);
      tx = NULL;
    }
  }

  [[noreturn]] void read_response_phase() {
    while (true) {
      transaction* tx = NULL;

      tx = m_r_response.read();

      std::cout << "read_response_phase" << std::endl;
      std::cout << "done" << std::endl;
      tx->done().notify();
    }
  }

  [[noreturn]] void write_data_phase() {
    while (true) {
      transaction* tx = m_w_data_fifo.read();
      m_w_response.write(tx);
      std::cout << "write_data_phase" << std::endl;
    }
  }

  [[noreturn]] void write_response_phase() {
    while (true) {
      transaction* tx;
      tx = m_w_response.read();
      tx->done().notify();
      std::cout << "done" << std::endl;
    }
  }
};
#endif  // TLM2AXI4_HPP
