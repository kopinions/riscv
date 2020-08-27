#ifndef INFERENCES_HPP
#define INFERENCES_HPP
#include "instruction.hpp"

template <isa::type, unsigned int WIDTH>
class inference {
 public:
  typename instruction<WIDTH>::type infer(isa::opcode, isa::func7, isa::func3){};
};

template <>
class inference<isa::type::RTYPE, 32> {
 public:
  typename instruction<32>::type infer(isa::opcode op, isa::func7 func7, isa::func3 func3) {
    switch (func7 << 3 | func3) {
      case 0b0000000000:
        return instruction<32>::type::ADD;
      default:
        return instruction<32>::type::UNSUPPORTED;
    }
  };
};

template <isa::type TYPE, unsigned int WIDTH>
class inferences {
 public:
  inference<TYPE, WIDTH> infer(fields<WIDTH>){

  };
};

template <>
class inferences<isa::type::RTYPE, 32> {
 public:
  typename instruction<32>::type infer(fields<32> fields) {
    return inference<isa::RTYPE, 32>{}.infer(fields.opcode(), fields.func7(), fields.func3());
  };
};
#endif  // INFERENCES_HPP
