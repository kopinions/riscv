#ifndef REGISTERS_HPP
#define REGISTERS_HPP
#include <systemc>
#include <bits.hpp>

template <unsigned int BITS = 32>
class registers : public sc_core::sc_module {
 public:
  using name = enum {
    PC
  };
  using type = typename bits_helper<normalize(BITS)>::type;

  registers(sc_core::sc_module_name mn) : sc_core::sc_module{mn}, m_bank{} {
    const std::initializer_list<name> names = {name::PC};
    for (auto n : names) {
      m_bank[n] = static_cast<type>(0);
    }
  }

  registers(const registers &) = delete;
  registers(registers &&) = delete;
  registers &operator=(registers &) = delete;
  registers &operator=(registers &&) = delete;

  type read(name name) { return m_bank[name]; };

  void write(name name, type value) { m_bank[name] = value; }

 private:
  std::map<name, type> m_bank;
};

#endif  // REGISTERS_HPP
