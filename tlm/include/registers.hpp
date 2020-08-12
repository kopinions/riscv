#ifndef REGISTERS_HPP
#define REGISTERS_HPP
#include <bits.hpp>
#include <systemc>

template <unsigned int COUNT, unsigned int BITS = 32>
class registers : public sc_core::sc_module {
 public:
  registers(sc_core::sc_module_name mn) : sc_core::sc_module{mn}, m_bank{} {
    for (auto i = 0; i < COUNT; i++) {
      m_bank[i] = static_cast<normalized<BITS>>(0);
    }
  }

  registers(const registers &) = delete;
  registers(registers &&) = delete;
  registers &operator=(registers &) = delete;
  registers &operator=(registers &&) = delete;

  normalized<BITS> read(unsigned int index) { return m_bank[index]; };

  void write(unsigned int index, normalized<BITS> value) { m_bank[index] = value; }

 private:
  std::map<unsigned int, normalized<BITS>> m_bank;
};

#endif  // REGISTERS_HPP
