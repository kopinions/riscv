#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <bits.hpp>
#include <isa.hpp>

TEST(isa, should_able_to_decode_the_r_instruction) {
  isa riscv{};
  // add x5,x6,x7
  const decoded<32>& decoded = riscv.decode<32>(normalized<32>{0x007302B3}, [](unsigned int reg_id) -> normalized<32> {
    switch (reg_id) {
      case 6:
        return normalized<32>{0x60000000};
      case 7:
        return normalized<32>{0x70000000};
      default:
        return normalized<32>{0};
    };
  });

  ASSERT_THAT(decoded.opcode, testing::Eq(0x33));
  ASSERT_THAT(decoded.rs1, testing::Eq(0x60000000));
  ASSERT_THAT(decoded.rs2, testing::Eq(0x70000000));
  ASSERT_THAT(decoded.rd_id, testing::Eq(5));
  ASSERT_THAT(decoded.func3, testing::Eq(0));
  ASSERT_THAT(decoded.func7, testing::Eq(0));
}

TEST(isa, should_able_to_decode_the_i_instruction) {
  isa riscv{};
  // addi x15,x1,-50
  const decoded<32>& decoded = riscv.decode<32>(normalized<32>{0xFCE08793}, [](unsigned int reg_id) -> normalized<32> {
    switch (reg_id) {
      case 1:
        return normalized<32>{0x60000000};
      default:
        return normalized<32>{0};
    };
  });

  ASSERT_THAT(decoded.opcode, testing::Eq(0x13));
  ASSERT_THAT(decoded.rs1, testing::Eq(0x60000000));
  ASSERT_THAT(decoded.rd_id, testing::Eq(15));
  ASSERT_THAT(decoded.imm, testing::Eq(0xFFFFFFCE));
}