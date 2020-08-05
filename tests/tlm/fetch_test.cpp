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
    while (true) {
      phase.raise_objection(this);
      start_sequence();
      phase.drop_objection(this);
      while (true) {
        wait(1);
      }
    }
  }

  virtual void start_of_simulation_phase(uvm::uvm_phase& phase) override {
    uv::test::start_of_simulation_phase(phase);
    uvm::print_topology();
  }
};
