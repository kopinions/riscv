#include <instruction.hpp>

#include "ram_sequence.hpp"
#include "rom_sequence.hpp"
#include "test.hpp"

class fetch_test : public uv::test {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_COMPONENT_UTILS(fetch_test)
  DISABLE_WARNING_POP

  explicit fetch_test(const std::string& name = "fetch_test") : uv::test{name} {}

 protected:
  void build_phase(uvm::uvm_phase& phase) override { uv::test::build_phase(phase); }

  void run_phase(uvm::uvm_phase& phase) override {
    phase.raise_objection(this);
    uv::instruction add{0, 0x007302B3};
    uv::instruction add1{4, 0x007302B3};
    uv::instruction end{8, 0x00000000};
    m_sequence->append(add);
    m_sequence->append(add1);
    m_sequence->append(end);
    m_sequence->start(m_sequencer);
    phase.drop_objection(this);
  }

  virtual void start_of_simulation_phase(uvm::uvm_phase& phase) override {
    uv::test::start_of_simulation_phase(phase);
    uvm::print_topology();
  }
};
