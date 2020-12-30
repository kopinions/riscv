#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <systemc.h>
#include <tlm_utils/simple_initiator_socket.h>

#include <variant>

#include "fake_receiver.hpp"
#include "fake_sender.hpp"
#include "recording.hpp"
using stype = std::variant<tlm::tlm_sync_enum, tlm::tlm_phase_enum>;

typedef std::map<tlm::tlm_generic_payload*, stype> waiting_bw_path_map;
waiting_bw_path_map m_bw_txs;
tlm_utils::peq_with_get<tlm::tlm_generic_payload> m_send_end_rsp_PEQ{"end_peq"};
sc_core::sc_event m_enable_next_request_event;
class mod : public sc_core::sc_module, public sponsor<tlm::tlm_base_protocol_types> {
  SC_HAS_PROCESS(mod);

 public:
  mod(const sc_module_name& nm) : sc_module(nm) {
//    o.register_nb_transport_bw();


    SC_METHOD(end_res);
    sensitive << m_send_end_rsp_PEQ.get_event();
    dont_initialize();
  }
  outbound<recordable_initiator_socket<>> o;

  tlm::tlm_sync_enum nb_transport_fw(outbound<recordable_initiator_socket<>>::transaction_type& payload,
                                     outbound<recordable_initiator_socket<>>::phase_type& phase,
                                     sc_core::sc_time&delay) {
    auto status = o->nb_transport_fw(payload, phase, delay);
    sc_core::sc_time m_end_rsp_delay = sc_core::sc_time(10, sc_core::SC_NS);

    switch (status) {
      case tlm::TLM_COMPLETED: {
        wait(delay + m_end_rsp_delay);
        EXPECT_THAT(std::string(reinterpret_cast<char*>(payload.get_data_ptr())), testing::Eq("done"));
        break;
      }
      case tlm::TLM_UPDATED: {
        if (phase == tlm::END_REQ) {
          auto p = std::make_pair(&payload, tlm::END_REQ);
          m_bw_txs.insert(p);
          wait(delay);  // wait annotated time
        } else if (phase == tlm::BEGIN_RESP) {
          wait(delay);
          m_send_end_rsp_PEQ.notify(payload, delay);
        } else {
          std::cerr << "error status";
        }
        break;
      }
      case tlm::TLM_ACCEPTED: {
        auto p = std::make_pair(&payload, tlm::TLM_ACCEPTED);
        m_bw_txs.insert(p);
        // TODO refactor needed to wait the event, change nb_transport_bw to peq
//        wait(m_enable_next_request_event);
      }
    }
  }

  void fulfilled(tlm_payload_type& type) override {

  }

 private:
  void end_res(){

  };
};

TEST(recordable_test, should_able_to_set_specific_bit) {
  scv_startup();
  scv_tr_text_init();
  scv_tr_db db("my_db.txlog");
  scv_tr_db::set_default_db(&db);
  bool received = false;
  fake_receiver<> rcv{"rcv",
                      [&received](fake_receiver<>::bw_interface_type* bw, auto& payload, tlm::tlm_phase& phase,
                                  auto& time) -> tlm::tlm_sync_enum {
                        received = true;
                        return tlm::TLM_ACCEPTED;
                      }};
  mod m{"xxx"};
  m.o.bind(rcv.inputs);
  tlm::tlm_generic_payload payload;
  std::string a = "test for data";
  payload.set_data_ptr((unsigned char*)a.c_str());
  payload.set_data_length(a.length());
  sc_core::sc_time delay = sc_core::sc_time(0, sc_core::SC_NS);
  tlm::tlm_phase phase = tlm::BEGIN_REQ;

//  m.nb_transport_fw(payload, phase, delay);
  m.o.transport(payload);

  sc_core::sc_start();
  EXPECT_TRUE(received);
}

int sc_main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}