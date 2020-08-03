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
  virtual void build_phase(uvm::uvm_phase& phase) override {
    uv::test::build_phase(phase);

    uvm::uvm_config_db<uvm::uvm_object_wrapper*>::set(this, "*.ram_sequencer.run_phase", "default_sequence",
                                                      uv::ram_sequence::type_id::get());
    uvm::uvm_config_db<uvm::uvm_object_wrapper*>::set(this, "*.rom_sequencer.run_phase", "default_sequence",
                                                      uv::rom_sequence::type_id::get());
  }

  virtual void run_phase(uvm::uvm_phase& phase) override {}

  virtual void start_of_simulation_phase(uvm::uvm_phase& phase) override {
    uv::test::start_of_simulation_phase(phase);
    uvm::print_topology();
  }
};
