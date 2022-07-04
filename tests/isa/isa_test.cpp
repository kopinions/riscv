#include "isa/isa.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "commons/bits.hpp"

TEST(isa, should_able_to_unpack_the_r_instruction) {
  isa riscv{};
  // add x5,x6,x7
  const fields<32>& fields = riscv.unpack<32>(bits::type<32>{0x007302B3});

  ASSERT_THAT(fields.type(), testing::Eq(isa::type::RTYPE));
  ASSERT_THAT(fields.opcode(), testing::Eq(0x33));
  ASSERT_THAT(fields.rs1(), testing::Eq(6));
  ASSERT_THAT(fields.rs2(), testing::Eq(7));
  ASSERT_THAT(fields.rd(), testing::Eq(5));
  ASSERT_THAT(fields.func3(), testing::Eq(0));
  ASSERT_THAT(fields.func7(), testing::Eq(0));
}

TEST(isa, should_able_to_unpack_the_i_instruction) {
  isa riscv{};
  // addi x15,x1,-50
  const fields<32>& fields = riscv.unpack<32>(bits::type<32>{0xFCE08793});

  ASSERT_THAT(fields.type(), testing::Eq(isa::type::ITYPE));
  ASSERT_THAT(fields.opcode(), testing::Eq(0x13));
  ASSERT_THAT(fields.rs1(), testing::Eq(1));
  ASSERT_THAT(fields.rd(), testing::Eq(15));
  ASSERT_THAT(fields.imm(), testing::Eq(0xFFFFFFCE));
}

TEST(isa, should_able_to_unpack_the_s_instruction) {
  isa riscv{};
  // sw x14, 8(x2)
  // 0000000 01110 00010 010 01000 0100011
  // 0000 0000 1110 0001 0010 0100 0010 0011
  // 0 0 E 1 2 4 2 3
  const fields<32>& fields = riscv.unpack<32>(bits::type<32>{0x00E12423});

  ASSERT_THAT(fields.type(), testing::Eq(isa::type::STYPE));
  ASSERT_THAT(fields.opcode(), testing::Eq(0x23));
  ASSERT_THAT(fields.rs2(), testing::Eq(14));
  ASSERT_THAT(fields.rs1(), testing::Eq(2));
  ASSERT_THAT(fields.func3(), testing::Eq(0x2));
  ASSERT_THAT(fields.imm(), testing::Eq(8));
}

TEST(isa, should_able_to_unpack_the_b_instruction) {
  isa riscv{};
  // beq x19, x11, 2324(0 1001 0001 0100)
  // 0001000 01011 10011 000 10101  1100011
  // 0001 0000 1011 1001 1000 1010 1110 0011
  // 1 0 B 9 8 A E 3
  const fields<32>& fields = riscv.unpack<32>(bits::type<32>{0x10B98AE3});

  ASSERT_THAT(fields.type(), testing::Eq(isa::type::BTYPE));
  ASSERT_THAT(fields.opcode(), testing::Eq(0x63));
  ASSERT_THAT(fields.rs2(), testing::Eq(11));
  ASSERT_THAT(fields.rs1(), testing::Eq(19));
  ASSERT_THAT(fields.func3(), testing::Eq(0x0));
  ASSERT_THAT(fields.imm(), testing::Eq(2324));
}

TEST(isa, should_able_to_unpack_the_u_instruction_lui) {
  isa riscv{};
  // lui x10, 0x254
  //  00000000000011111110 01010  0110111
  // 0000 0000 0000 1111 1110 0101 0011 0111
  // 0 0 0 F E 5 3 7
  const fields<32>& fields = riscv.unpack<32>(bits::type<32>{0x000FE537});

  ASSERT_THAT(fields.type(), testing::Eq(isa::type::UTYPE));
  ASSERT_THAT(fields.opcode(), testing::Eq(0x37));
  ASSERT_THAT(fields.rd(), testing::Eq(10));
  ASSERT_THAT(fields.imm(), testing::Eq(254));
}

TEST(isa, should_able_to_unpack_the_u_instruction_auipc) {
  isa riscv{};
  // auipc x10, 0x254
  //  00000000000011111110 01010  0010111
  // 0000 0000 0000 1111 1110 0101 0001 0111
  // 0 0 0 F E 5 1 7
  const fields<32>& fields = riscv.unpack<32>(bits::type<32>{0x000FE517});

  ASSERT_THAT(fields.type(), testing::Eq(isa::type::UTYPE));
  ASSERT_THAT(fields.opcode(), testing::Eq(0x17));
  ASSERT_THAT(fields.rd(), testing::Eq(10));
  ASSERT_THAT(fields.imm(), testing::Eq(254));
}

TEST(isa, should_able_to_unpack_the_j_instruction) {
  isa riscv{};
  // jal x10, 0x254(0000 0000 0000 1111 1110)
  //  00001111111000000000 01010  1101111
  // 0000 1111 1110 0000 0000 0101 0110 1111
  // 0 F E 0 0 5 6 F
  const fields<32>& fields = riscv.unpack<32>(bits::type<32>{0x0FE0056F});

  ASSERT_THAT(fields.type(), testing::Eq(isa::type::JTYPE));
  ASSERT_THAT(fields.opcode(), testing::Eq(0x6F));
  ASSERT_THAT(fields.rd(), testing::Eq(10));
  ASSERT_THAT(fields.imm(), testing::Eq(254));
}

int sc_main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}