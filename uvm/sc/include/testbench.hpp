#ifndef TESTBENCH_HPP
#define TESTBENCH_HPP
#include <uvm>

namespace uv {
  template<class...Ts>
  struct testbench;
  template<class T, class...Ts>
  struct testbench<T, Ts...>;

  template <size_t, class> struct elem_type_holder {};

  template <class T, class... Ts>
  struct elem_type_holder<0, testbench<T, Ts...>> {
    typedef T type;
  };

  template <size_t k, class T, class... Ts>
  struct elem_type_holder<k, testbench<T, Ts...>> {
    typedef typename elem_type_holder<k - 1, testbench<Ts...>>::type type;
  };

  template<class...Ts>
  struct testbench {
  };
  
  template<class T, class...Ts>
  struct testbench<T, Ts...>: testbench<Ts...>, public uvm::uvm_env {
    UVM_OBJECT_UTILS(uv::testbench<T, Ts...>)
    

  public:
    testbench(T t, Ts... ts): testbench(ts...), tail{t} {}
    virtual void build_phase(uvm::uvm_phase& phase) override {
      testbench<Ts...>::build_phase(phase);
      tail = T::type_id::create(T::m_register_type_name(), this);
    }

    virtual void connect_phase(uvm::uvm_phase& phase) override {
      
      
    }
    T tail;
    T *ptr;
  };
}
#endif //TESTBENCH_HPP
