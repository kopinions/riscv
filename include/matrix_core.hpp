#ifndef MATRIX_CORE_HPP
#define MATRIX_CORE_HPP
#include <systemc>

#include "matrix_system.hpp"
#include "ram.hpp"
#include "rom.hpp"

class matrix_core : public sc_core::sc_module {
 public:
  matrix_core(const sc_core::sc_module_name& name) : sc_core::sc_module{name} {}

 private:
  matrix_system* m_system;
  ram* m_ram;
  rom* m_rom;
};

#endif  // MATRIX_TOP_HPP
