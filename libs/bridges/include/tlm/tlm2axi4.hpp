#ifndef TLM2AXI4_HPP
#define TLM2AXI4_HPP
#include <tlm_utils/simple_target_socket.h>
#define TLM2AXI_BRIDGE_MSG "tlm2axi-bridge"
#include <systemc>

#include "../../include/commons/bits.hpp"
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

  sc_core::sc_in<bool> m_clk;
  sc_core::sc_in<bool> m_resetn;
  sc_core::sc_out<bool> m_arvalid;
  sc_core::sc_in<bool> m_arready;
  sc_core::sc_out<address_type> m_araddr;

  sc_core::sc_in<bool> m_rvalid;
  sc_core::sc_out<bool> m_rready;
  sc_core::sc_in<data_type> m_rdata;

  template <typename T>
  void abort(T* tr) {
    tlm::tlm_generic_payload& trans = tr->payload();
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    tr->done().notify();
  }

  template <typename T>
  void tlm2axi_clear_fifo(sc_core::sc_fifo<T*>& fifo) {
    while (fifo.num_available() > 0) {
      T* tr = fifo.read();

      abort(tr);
    }
  }

  void wait_for_reset_release() {
    do {
      sc_core::wait(m_clk.posedge_event());
    } while (m_resetn.read() == 0);
  }

  void wait_abort_on_reset(sc_core::sc_in<bool>& sig) {
    do {
      sc_core::wait(m_clk.posedge_event() | m_resetn.negedge_event());
    } while (sig.read() == false && m_resetn.read() == true);
  }

  bool is_reset() { return m_resetn.read() == false; }

  tlm2axi4(const sc_core::sc_module_name& name) : sc_module(name) {
    m_target.register_b_transport(this, &tlm2axi4<ADDR_WIDTH, DATA_WIDTH>::b_transport);

    tlm2axi_clear_fifo(m_r_transaction_fifo);
    tlm2axi_clear_fifo(m_r_response);
    tlm2axi_clear_fifo(m_w_transaction_fifo);
    tlm2axi_clear_fifo(m_w_data_fifo);
    tlm2axi_clear_fifo(m_w_response);

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

    bool is_valid() {
      if (m_payload.get_data_length() == 0) {
        SC_REPORT_INFO(TLM2AXI_BRIDGE_MSG, "Zero-length transaction");
        return false;
      }

      if (m_payload.get_streaming_width() == 0) {
        SC_REPORT_INFO(TLM2AXI_BRIDGE_MSG, "Zero-length streaming-width");
        return false;
      }

      if (!m_payload.is_read() && !m_payload.is_write()) {
        return false;
      }
      return true;
    };

    std::uint64_t address() { return m_payload.get_address(); }

    std::size_t data_length() const { return m_payload.get_data_length(); }

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
  void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
    transaction tx{trans};

    sc_core::wait(delay, m_resetn.negedge_event());
    delay = sc_core::SC_ZERO_TIME;

    m_mutex.lock();

    if (m_resetn.read() && tx.is_valid()) {
      if (trans.is_read()) {
        m_r_transaction_fifo.write(&tx);
      } else {
        m_w_transaction_fifo.write(&tx);
      }

      wait(tx.done());
    } else {
      trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    }

    m_mutex.unlock();
  }

  [[noreturn]] void read_address_phase() {
    while (true) {
      SC_REPORT_INFO(TLM2AXI_BRIDGE_MSG, "read_address_phase start");

      transaction* tx = m_r_transaction_fifo.read();
      if (is_reset()) {
        m_arvalid.write(false);
        continue;
      }

      m_araddr.write(tx->address());
      m_arvalid.write(true);

      wait_abort_on_reset(m_arready);

      m_arvalid.write(false);

      if (!is_reset()) {
        m_r_response.write(tx);
        SC_REPORT_INFO(TLM2AXI_BRIDGE_MSG, "read_address_phase done");
      }
    }
  }

  [[noreturn]] void read_response_phase() {
    m_rready.write(false);
    while (true) {
      SC_REPORT_INFO(TLM2AXI_BRIDGE_MSG, "read_response_phase start");
      transaction* tx = nullptr;
      tlm::tlm_generic_payload* payload = nullptr;
      auto len = 0;
      while (tx == nullptr || len > 0) {
        m_rready.write(true);
        wait(m_clk.posedge_event() | m_resetn.negedge_event());
        if (is_reset()) {
          break;
        }

        if (m_rvalid.read()) {
          tx = m_r_response.read();
          len = tx->data_length();
          payload = &tx->payload();
          auto addr = payload->get_address();
          auto data_ptr = payload->get_data_ptr();
          auto bitoffset = (addr * 8) % DATA_WIDTH;
          auto readlen = (DATA_WIDTH - bitoffset) / 8;
          readlen = readlen <= len ? readlen : len;
          auto position = 0;
          using read_data_type = typename bits::bits_helper<bits::normalize(ADDR_WIDTH)>::type;
          for (auto i = 0; i < readlen; i += DATA_WIDTH / 8) {
            const data_type& data = m_rdata.read() >> (i * 8 + bitoffset);
            auto copylen = (readlen - i) <= sizeof(data) ? readlen - i : sizeof(data);
            auto assign = [](const data_type& data) -> read_data_type {
              read_data_type v = 0;
              for (auto i = 0; i < sizeof(v) * 8; i++) {
                v |= (data.get_bit(i) << i);
              }
              return v;
            };
            read_data_type read_data = assign(data);
            SC_REPORT_INFO(TLM2AXI_BRIDGE_MSG, ("read_response_phase done" + std::to_string(read_data)).c_str());
            memcpy(data_ptr + position, &read_data, copylen);
            position += copylen;
            len -= copylen;
          }
        }
      }
      m_rready.write(false);

      if (!is_reset()) {
        payload->set_response_status(tlm::TLM_OK_RESPONSE);
        tx->done().notify();
        SC_REPORT_INFO(TLM2AXI_BRIDGE_MSG, "read_response_phase done");
      } else {
        tx = nullptr;
        wait_for_reset_release();
      }
    }
  }

  [[noreturn]] void write_address_phase() {
    while (true) {
      transaction* tx = m_w_transaction_fifo.read();
      m_w_data_fifo.write(tx);
      tx = NULL;
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
