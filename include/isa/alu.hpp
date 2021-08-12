#ifndef ALU_HPP
#define ALU_HPP
#include <instruction.hpp>


template <unsigned int WIDTH>
class alu {
 public:
  virtual typename instruction<WIDTH>::result evaluate(instruction<WIDTH> inst) const {
    typename instruction<WIDTH>::result res = typename instruction<WIDTH>::result{};
    switch (inst.typ) {
      case instruction<WIDTH>::type::ADD: {
        res.wb = true;
        res.res = inst.rs1 + inst.rs2;
        break;
      }
      default:
        break;
    }
    return res;
  };
};
#include "instruction.hpp"
#endif  // ALU_HPP
