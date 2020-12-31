#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <systemc.h>
#include <tlm_utils/simple_initiator_socket.h>

#include <variant>

#include "fake_receiver.hpp"
#include "fake_sender.hpp"
#include "recording.hpp"

class sender : public sc_core::sc_module, public sponsor<tlm::tlm_base_protocol_types> {
  SC_HAS_PROCESS(sender);

 public:
  sender(const sc_module_name& nm) : sc_module(nm) {}
  outbound<recordable_initiator_socket<>> o;

  void fulfilled(tlm_payload_type& type) override {
    m_result = std::string{reinterpret_cast<char*>(type.get_data_ptr())};
  }

  std::string result() { return m_result; }

 private:
  std::string m_result;
};

class receiver : public sc_core::sc_module, public sponsee<tlm::tlm_base_protocol_types> {
 public:
  inbound<recordable_target_socket<>> i;
  receiver(const sc_module_name& nm) : sc_module(nm) { i.sponsed(this); }

  void fulfill(tlm_payload_type& type) override { type.set_data_ptr((unsigned char*)"req for data"); }
};

TEST(recordable_test, should_able_to_set_specific_bit) {
  scv_startup();
  scv_tr_text_init();
  scv_tr_db db("my_db.txlog");
  scv_tr_db::set_default_db(&db);
  receiver rcv{"receiver"};
  sender sen{"sender"};
  sen.o.bind(rcv.i);

  tlm::tlm_generic_payload payload;
  std::string a = "req for data";
  payload.set_data_ptr((unsigned char*)a.c_str());
  payload.set_data_length(a.length());

  sen.o.transport(payload);

  sc_core::sc_start();
  EXPECT_THAT(sen.result(), testing::Eq("test for data"));
}

int sc_main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}