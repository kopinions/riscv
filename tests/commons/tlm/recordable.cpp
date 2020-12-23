#include <gtest/gtest.h>

#include "fake_receiver.hpp"
#include "fake_sender.hpp"
#include "recording.hpp"

TEST(recordable_test, should_able_to_set_specific_bit) {
  fake_receiver rcv{"rcv"};
  outbound<> o;
  o.bind(rcv.inputs);
  tlm::tlm_generic_payload payload;
  sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
  o->b_transport(payload, delay);
}

int sc_main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}