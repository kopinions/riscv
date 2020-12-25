#include <gtest/gtest.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "fake_receiver.hpp"
#include "fake_sender.hpp"
#include "recording.hpp"

class mod : public sc_core::sc_module {
 public:
  mod(const sc_module_name& nm) : sc_module(nm) {}
  outbound<recordable_initiator_socket<>> o;
  //  tlm_utils::simple_initiator_socket<mod> o;
};

TEST(recordable_test, should_able_to_set_specific_bit) {
  scv_startup();
  scv_tr_text_init();
  scv_tr_db db("my_db.txlog");
  scv_tr_db::set_default_db(&db);
  bool received = false;
  fake_receiver rcv{"rcv", [&received](auto& payload, auto& time) -> void { received = true; }};
  mod m{"xxx"};
  m.o.bind(rcv.inputs);
  tlm::tlm_generic_payload payload;
  std::string a = "test for data";
  payload.set_data_ptr((unsigned char*)a.c_str());
  payload.set_data_length(a.length());
  sc_core::sc_time delay = sc_core::sc_time(100, sc_core::SC_NS);
  tlm::tlm_phase phase = tlm::BEGIN_REQ;
  m.o->nb_transport_fw(payload, phase, delay);
  sc_core::sc_start();
  EXPECT_TRUE(received);
}

int sc_main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}