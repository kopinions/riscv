#pragma once
#include <tlm.h>
template <typename T>
class singleton {
 public:
  template <typename... Args>
  static T &get(Args... args) {
    static T t{std::forward<Args>(args)...};
    return t;
  }
};

template <typename TYPES = tlm::tlm_base_protocol_types>
class payload_memory_manager : public tlm::tlm_mm_interface {
  using payload_type = typename TYPES::tlm_payload_type;

 public:
  payload_memory_manager() = default;

  payload_type *alloc() {
    payload_type *p;
    if (m_pool.empty()) {
      p = new payload_type(this);
    } else {
      // Get the pointer from the recycling pool
      p = m_pool.back();
      // Remove the pointer from recycling pool
      m_pool.pop_back();
    }
    return p;
  }

  void free(tlm::tlm_generic_payload *payload) override {
    if (payload->get_data_ptr()) {
      delete[] payload->get_data_ptr();
    }

    payload->set_data_ptr(nullptr);
    payload->reset();
    payload->~tlm_generic_payload();

    // Add payload to recycle pool
    m_pool.emplace_back((payload_type *)payload);
  }
  ~payload_memory_manager() override {
    for (auto *p : m_pool) {
      delete p;
    }
  }

 private:
  std::vector<payload_type *> m_pool;
};