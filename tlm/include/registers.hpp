#ifndef REGISTERS_HPP
#define REGISTERS_HPP
#include <systemc>
template <unsigned int N>
struct registers_helper {
  using type = std::uint32_t;
};

template <>
struct registers_helper<64> {
  using type = std::uint64_t;
};

template <>
struct registers_helper<32> {
  using type = std::uint32_t;
};

template <>
struct registers_helper<16> {
  using type = std::uint16_t;
};

template <unsigned int BITS = 32>
class registers : public sc_core::sc_module {
 public:
  enum class name { PC };
  using type = typename registers_helper<(BITS <= 16 ? 16 : BITS <= 32 ? 32 : BITS <= 64 ? 64 : BITS)>::type;

  registers(sc_core::sc_module_name mn) : sc_core::sc_module{mn}, m_bank{} {
    const std::initializer_list<name> names = {name::PC};
    for (auto n : names) {
      m_bank[n] = static_cast<type>(0);
    }
  }

  type read(name name) { return m_bank[name]; };

  void write(name name, type value) { m_bank[name] = value; }

 private:
  std::map<name, type> m_bank;
};

#endif  // REGISTERS_HPP
