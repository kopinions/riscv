#ifndef IBUS_HPP
#define IBUS_HPP
#include "bitstream.hpp"

class ibus : public sc_core::sc_module {
 public:
  explicit ibus(sc_core::sc_module_name& name) : sc_core::sc_module{name} {};
  virtual std::uint8_t read(const bitstream& addr, std::size_t offset) const = 0;
  virtual void write(const bitstream& addr, std::size_t offset, std::uint8_t value) = 0;

  ibus(ibus&&) = delete;
  ibus(const ibus&) = delete;
  ibus& operator=(const ibus&) = delete;
  ibus& operator=(ibus&&) = delete;

  virtual ~ibus() = default;
};

#endif  // IBUS_HPP
