#pragma once

#include <scv.h>
#include <tlm.h>

class recording_extension : public virtual tlm::tlm_extension<recording_extension> {
 public:
  recording_extension(scv_tr_handle tx_manager, void* const creator)
      : m_tx_manager{tx_manager}, m_created_by{creator} {}

  [[nodiscard]] tlm_extension_base* clone() const override {
    auto* t = new recording_extension(this->m_tx_manager, this->m_created_by);
    return t;
  }

  void copy_from(const tlm_extension_base& ext) override {}

  ~recording_extension() override = default;

  void* created_by() { return m_created_by; }

 private:
  scv_tr_handle m_tx_manager;
  void* m_created_by;
};