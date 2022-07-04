#include "commons/bitstream.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(bitstream, should_able_to_set_specific_bit) {
  bitstream bits(3);
  bits[2] = true;
  ASSERT_THAT(bool(bits[2]), testing::Eq(true));

  bits[2] = false;
  ASSERT_THAT(bool(bits[2]), testing::Eq(false));
}

TEST(bitstream, should_able_to_set_specific_bit_with_const) {
  const bitstream bits(3);
  bits[2] = true;
  const auto second = bits[2];
  ASSERT_THAT(bool(second), testing::Eq(true));

  bits[2] = false;
  ASSERT_THAT(bool(bits[2]), testing::Eq(false));
}

int sc_main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}