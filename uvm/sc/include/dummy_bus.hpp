#ifndef DUMMY_BUS_HPP
#define DUMMY_BUS_HPP
#include "ibus.hpp"

template <std::size_t ADDR_WIDTH = 64, std::size_t DATA_WIDTH = 64>
class dummy_bus : public ibus {
 public:
  using address_type = sc_dt::sc_bv<ADDR_WIDTH>;
  using data_type = sc_dt::sc_bv<DATA_WIDTH>;

  dummy_bus(const sc_core::sc_module_name& name) : ibus(name) {}

  std::uint8_t read(const bitstream& addr, std::size_t offset) const override {
    address_type m_addr{};
    data_type m_data{};
    auto size = (addr.size() < ADDR_WIDTH) ? addr.size() : ADDR_WIDTH;
    for (auto i = 0u; i < size; i++) {
      m_addr[i] = bool(addr[i]);
    }
    s_addr.write(m_addr);
    data_type data = s_data.read();
    return std::uint8_t(data(int(offset * 8), int(offset)).to_uint());
  };

  void write(const bitstream& addr, std::size_t offset, std::uint8_t value) override {
    address_type m_addr{};
    data_type m_data{};
    auto size = (addr.size() < ADDR_WIDTH) ? addr.size() : ADDR_WIDTH;
    for (auto i = 0u; i < size; i++) {
      m_addr[i] = bool(addr[i]);
    }
    s_addr.write(m_addr);
    m_data[8u * offset] = value;
    s_data.write(m_data);
  };

 private:
  mutable sc_core::sc_signal<address_type> s_addr{"addr"};
  sc_core::sc_signal<data_type> s_data{"data"};
};
#endif  // DUMMY_BUS_HPP
