#ifndef MATRIX_TOP_HPP
#define MATRIX_TOP_HPP
#include <systemc>
#include <tlm>

#include "matrix.hpp"
#include "matrix_core.hpp"
#include "ram.hpp"
#include "rom.hpp"

class matrix_top : public sc_core::sc_module {
 public:
  matrix_top(const sc_core::sc_module_name& name) : sc_core::sc_module{name} {}

 private:
  matrix_core* m_core;
  ram* m_ram;
  rom* m_rom;
};

#endif  // MATRIX_TOP_HPP
