#ifndef BITSTREAM_HPP
#define BITSTREAM_HPP
#include <cstdint>
#include <cstdlib>
#include <type_traits>
using voidptr = void *;

static constexpr std::size_t BYTE_LENGTH = 8;
static constexpr std::size_t size(std::size_t size) { return size / BYTE_LENGTH + size % BYTE_LENGTH == 0 ? 0 : 1; }

class bitstream_ref {
 public:
  bitstream_ref(voidptr bits, std::size_t index) noexcept : m_bits{bits}, m_index{index} {}

  bitstream_ref &operator=(bool value) noexcept {
    auto mask = std::uint8_t(1 << (m_index % BYTE_LENGTH));
    auto data = static_cast<std::uint8_t *>(m_bits) + (m_index / BYTE_LENGTH);

    if (value) {
      *data |= mask;
    } else {
      *data &= std::uint8_t(~mask);
    }

    return *this;
  }
  explicit operator bool() const noexcept {
    auto mask = std::uint8_t(1 << (m_index % BYTE_LENGTH));
    auto data = static_cast<std::uint8_t *>(m_bits) + (m_index / BYTE_LENGTH);
    return (*data & mask) == mask;
  };

 private:
  voidptr m_bits;
  std::size_t m_index;
};
class bitstream_const_ref {
 public:
  bitstream_const_ref(const voidptr bits, std::size_t index) noexcept : m_bits{bits}, m_index{index} {}

  bitstream_const_ref(bitstream_const_ref &&) noexcept = default;

  bitstream_const_ref(bitstream_const_ref &) noexcept = default;

  bitstream_const_ref &operator=(bool value) noexcept {
    auto mask = std::uint8_t(1 << (m_index % BYTE_LENGTH));
    auto data = static_cast<std::uint8_t *>(m_bits) + (m_index / BYTE_LENGTH);
    *data = value ? *data | mask : *data & std::uint8_t(~mask);
    return *this;
  }

  explicit operator bool() const noexcept {
    auto mask = std::uint8_t(1 << (m_index % BYTE_LENGTH));
    auto data = static_cast<std::uint8_t *>(m_bits) + (m_index / BYTE_LENGTH);
    return (*data & mask) == mask;
  };

 private:
  const voidptr m_bits;
  std::size_t m_index;
};

class bitstream {
 public:
  bitstream() : m_bits{nullptr}, m_size{0} {}
  bitstream(std::size_t size) : m_bits{new std::uint8_t[::size(size)]{}}, m_size{size} {}

  std::size_t size() const { return m_size; };

  bitstream_ref operator[](std::size_t index) noexcept { return {m_bits, index}; };

  bitstream_const_ref operator[](std::size_t index) const noexcept { return {m_bits, index}; };

  template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
  explicit operator T() const noexcept {
    auto bytes = static_cast<const std::uint8_t *>(m_bits);
    std::size_t offset = 0;
    std::uintmax_t val = 0;
    std::size_t bits;

    if (m_size < 8 * sizeof(std::uintmax_t)) {
      bits = m_size;
    }

    while (bits >= 8) {
      val |= (std::uintmax_t(*bytes++) << offset);
      offset += 8;
      bits -= 8;
    }

    if (bits > 0) {
      auto mask = std::uint8_t(~(0xFF << bits));
      val |= (std::uintmax_t(mask & (*bytes++)) << offset);
    }

    return T(val);
  };

 private:
  voidptr m_bits;
  std::size_t m_size{};
};
#endif  // BITSTREAM_HPP
