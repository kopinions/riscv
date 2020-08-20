#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <bits.hpp>
#include <isa.hpp>

TEST(isa, should_able_to_decode_the_r_instruction) {
  isa riscv{};
  // add x5,x6,x7
  const fields<32>& fields = riscv.unpack<32>(bits::type<32>{0x007302B3});

  ASSERT_THAT(fields.type, testing::Eq(isa::type::RTYPE));
  ASSERT_THAT(fields.opcode, testing::Eq(0x33));
  ASSERT_THAT(fields.rs1, testing::Eq(6));
  ASSERT_THAT(fields.rs2, testing::Eq(7));
  ASSERT_THAT(fields.rd, testing::Eq(5));
  ASSERT_THAT(fields.func3, testing::Eq(0));
  ASSERT_THAT(fields.func7, testing::Eq(0));
}

TEST(isa, should_able_to_decode_the_i_instruction) {
  isa riscv{};
  // addi x15,x1,-50
  const fields<32>& decoded = riscv.unpack<32>(bits::type<32>{0xFCE08793});

  ASSERT_THAT(decoded.type, testing::Eq(isa::type::ITYPE));
  ASSERT_THAT(decoded.opcode, testing::Eq(0x13));
  ASSERT_THAT(decoded.rs1, testing::Eq(1));
  ASSERT_THAT(decoded.rd, testing::Eq(15));
  ASSERT_THAT(decoded.imm, testing::Eq(0xFFFFFFCE));
}