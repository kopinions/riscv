#ifndef BITS_HPP
#define BITS_HPP
namespace bits {

template <unsigned int N>
struct bits_helper {
  using type = uint32_t;
  using signed_type = uint32_t;
};

template <>
struct bits_helper<64> {
  using type = uint64_t;
  using signed_type = uint64_t;
};

template <>
struct bits_helper<32> {
  using type = uint32_t;
  using signed_type = int32_t;
};

template <>
struct bits_helper<16> {
  using type = uint16_t;
  using signed_type = int16_t;
};

static constexpr unsigned int normalize(unsigned int BITS) {
  return BITS <= 16 ? 16 : BITS <= 32 ? 32 : BITS <= 64 ? 64 : BITS;
}

template <unsigned int WIDTH = 32>
using type = typename bits::bits_helper<bits::normalize(WIDTH)>::type;

template <unsigned int WIDTH = 32>
using signed_type = typename bits::bits_helper<bits::normalize(WIDTH)>::signed_type;
}  // namespace bits

#endif  // BITS_HPP
