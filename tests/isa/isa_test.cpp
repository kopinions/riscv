#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <bits.hpp>
#include <isa.hpp>

TEST(isa, should_able_to_get_opcode) {
  isa riscv{};
  const decoded<32>& decoded = riscv.decode<32>(normalized<32>{0x007302B3}, [](unsigned int reg_id) -> normalized<32> {
    switch (reg_id) {
      case 5:
        return normalized<32>{5};
      case 6:
        return normalized<32>{6};
      case 7:
        return normalized<32>{7};
      default:
        return normalized<32>{0};
    };
  });
  ASSERT_THAT(decoded.opcode, testing::Eq(0x33));
}