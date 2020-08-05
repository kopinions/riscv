#ifndef BITS_HPP
#define BITS_HPP
template <unsigned int N>
struct bits_helper {
  using type = std::uint32_t;
};

template <>
struct bits_helper<64> {
  using type = std::uint64_t;
};

template <>
struct bits_helper<32> {
  using type = std::uint32_t;
};

template <>
struct bits_helper<16> {
  using type = std::uint16_t;
};
#endif  // BITS_HPP
