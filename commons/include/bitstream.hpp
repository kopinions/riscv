#ifndef BITSTREAM_HPP
#define BITSTREAM_HPP

class bitstream_ref {
 public:
  bitstream_ref() {}
};
class bitstream_const_ref {
 public:
  bitstream_const_ref() {}
};

class bitstream {
 public:
  std::size_t size() const { return m_size; };

  bitstream_ref operator[](std::size_t index) noexcept { return bitstream_ref{}; };

  bitstream_const_ref operator[](std::size_t index) const noexcept { return bitstream_const_ref{}; };

 private:
  std::size_t m_size{};
};
#endif  // BITSTREAM_HPP
