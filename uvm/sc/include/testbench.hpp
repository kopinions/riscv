#ifndef TESTBENCH_HPP
#define TESTBENCH_HPP
#include <uvm>

namespace uv {

template <class... Ts>
struct testbench : public uvm::uvm_env {
  UVM_OBJECT_UTILS(uv::testbench<Ts...>)
  testbench() : uvm::uvm_env{uvm::uvm_component_name{"testbench"}} {}
};

template <class T, class... Ts>
struct testbench<T, Ts...> : public testbench<Ts...> {
  testbench(T* t, Ts*... ts) : testbench<Ts...>(ts...), tail{t} {}
  T* tail;

  virtual void build_phase(uvm::uvm_phase& phase) override {
    testbench<Ts...>::build_phase(phase);
    tail = T::type_id::create(T::m_register_type_name(), this);
  }

  virtual void connect_phase(uvm::uvm_phase& phase) override {}
};

}  // namespace uv
#endif  // TESTBENCH_HPP
