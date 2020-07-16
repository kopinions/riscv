#ifndef BINDER_HPP
#define BINDER_HPP
#include <iostream>
#include <type_traits>

#include "matrix.hpp"
#include "timer.hpp"

// template <typename Initializer, typename Target>
// void bind(Initializer, Target){
//
//};
template <typename Initializer, typename Target,
          typename std::enable_if<std::is_same<Initializer, matrix>::value && std::is_same<Target, timer>::value,
                                  matrix>::type>
void bind(matrix* system, timer* timer) {
  std::cout << "test " << std::endl;
};

#endif  // BINDER_HPP
