#ifndef MM_HPP
#define MM_HPP
#include <systemc>

class mm : public sc_core::sc_module {
 public:
  mm(const sc_core::sc_module_name& name) noexcept : sc_core::sc_module{name} {}

 public:
  tlm_utils::simple_initiator_socket<mm> m_initiator;
  tlm_utils::simple_target_socket<mm> m_target;
  const bitstream read(bitstream addr) { return bitstream{}; };

  void write(bitstream addr, bitstream value) const {

  };
};

#endif  // MM_HPP
