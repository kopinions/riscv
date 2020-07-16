#include "ram_sequence.hpp"
#include "test.hpp"

class fetch_test : public uv::test {
 public:
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winconsistent-missing-override"
  UVM_COMPONENT_UTILS(fetch_test)
#pragma GCC diagnostic pop
  explicit fetch_test(const std::string& name = "fetch_test") : uv::test{name} {}

 protected:
  virtual void build_phase(uvm::uvm_phase& phase) override {
    uv::test::build_phase(phase);
    auto ram_seq = uv::ram_sequence::type_id::create("ram_sequence", this);
    if (ram_seq == nullptr) {
      UVM_FATAL(get_name(),
                "Cannot create ram sequence!"
                " Simulation aborted!");
    }
    uvm::uvm_config_db<uv::ram_sequence*>::set(this, "*.m_sequencer.run_phase", "default_sequence", ram_seq);
  }

  virtual void run_phase(uvm::uvm_phase& phase) override {}

  virtual void start_of_simulation_phase(uvm::uvm_phase& phase) override {
    uv::test::start_of_simulation_phase(phase);
    uvm::print_topology();
  }
};
