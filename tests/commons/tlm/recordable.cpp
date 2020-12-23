#include <gtest/gtest.h>

#include "fake_receiver.hpp"
#include "fake_sender.hpp"
#include "recording.hpp"

class mod : public sc_core::sc_module {
 public:
  mod(const sc_module_name& nm) : sc_module(nm) {}
  outbound<recordable_initiator_socket<>> o;
};

TEST(recordable_test, should_able_to_set_specific_bit) {
  fake_receiver rcv{"rcv"};
  mod m{"xxx"};
  m.o.bind(rcv.inputs);
  tlm::tlm_generic_payload payload;
  sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
  m.o->b_transport(payload, delay);
}

int sc_main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}