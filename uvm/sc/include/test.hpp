#ifndef TEST_HPP
#define TEST_HPP
#include <uvm>

#include "testbench.hpp"

namespace uv {

class test : public uvm::uvm_test {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_COMPONENT_UTILS(uv::test)
  DISABLE_WARNING_POP

  test() : uvm::uvm_test{uvm::uvm_component_name{"test"}}, m_testbench{nullptr} {}

  test(const std::string& name = "test") : uvm::uvm_test{uvm::uvm_component_name{name.c_str()}}, m_testbench{nullptr} {}

  virtual void build_phase(uvm::uvm_phase& phase) override {
    uvm::uvm_test::build_phase(phase);
    m_testbench = testbench::type_id::create("testbench", this);

    if (m_testbench == nullptr) {
      UVM_FATAL(get_name(),
                "Cannot create testbench!"
                " Simulation aborted!");
    }
  }

  virtual void run_phase(uvm::uvm_phase& phase) override {
    uvm::uvm_test::run_phase(phase);
    UVM_INFO(get_name(), "Run phase", uvm::UVM_FULL);

    phase.raise_objection(this);

    //    m_sequence->start(m_testbench->sequencer);

    phase.drop_objection(this);
  }

  void extract_phase(uvm::uvm_phase& phase) override {
    uvm::uvm_test::extract_phase(phase);
    uvm::uvm_config_db<bool>::set(nullptr, "*", "test_passed", true);
  }

  void report_phase(uvm::uvm_phase& phase) override {
    uvm::uvm_test::report_phase(phase);

    UVM_INFO(get_name(), "TEST PASSED", uvm::UVM_FULL);

    uvm::uvm_root::get()->set_finish_on_completion(true);
    uvm::uvm_report_server::get_server()->report_summarize();
    sc_core::sc_stop();
  }

 private:
  testbench* m_testbench;
};
}  // namespace uv
#endif  // TEST_HPP
