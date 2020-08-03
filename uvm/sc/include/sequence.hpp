#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include "sequencer.hpp"
namespace uv {
class sequence : public uvm::uvm_sequence<> {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_OBJECT_UTILS(uv::sequence)
  DISABLE_WARNING_POP

  sequence() : uvm::uvm_sequence<>{"sequence"} {};

  explicit sequence(const std::string& name)
      : uvm::uvm_sequence<>{name},
        m_rom_sequence{nullptr},
        m_ram_sequence{nullptr},
        m_ram_sequencer{nullptr},
        m_rom_sequencer{nullptr} {
    m_ram_sequence = ram_sequence::type_id::create("ram_sequence", nullptr);
    if (m_ram_sequence == nullptr) {
      UVM_FATAL(get_name(), "Cannot create ram sequence! Simulation aborted!");
    }

    m_rom_sequence = rom_sequence::type_id::create("rom_sequence", nullptr);
    if (m_rom_sequence == nullptr) {
      UVM_FATAL(get_name(), "Cannot create rom sequence! Simulation aborted!");
    }
  }

  sequence(sequence&&) = delete;

  sequence(const sequence&) = delete;

  sequence& operator=(sequence&&) = delete;

  sequence& operator=(const sequence&) = delete;

  ~sequence() override = default;

 protected:
  void pre_body() override {
    auto starting_phase = get_starting_phase();
    if (starting_phase != nullptr) {
      starting_phase->raise_objection(this);
    }

    auto sequencer = dynamic_cast<class sequencer*>(get_sequencer());
    if (sequencer != nullptr) {
      m_ram_sequencer = sequencer->m_ram_sequencer;
      m_rom_sequencer = sequencer->m_rom_sequencer;
    } else {
      UVM_FATAL(get_name(), "Cannot get sequencer! Simulation aborted!");
    }
  };

  void body() override {
    UVM_INFO(get_name(), "Starting sequence", uvm::UVM_FULL);

    sc_core::sc_process_handle forkees[] = {
        sc_core::sc_spawn(sc_bind([&]() { m_ram_sequence->start(m_ram_sequencer); }), "ram_sequence_handler", nullptr),
        sc_core::sc_spawn(sc_bind([&]() { m_rom_sequence->start(m_rom_sequencer); }), "rom_sequence_handler", nullptr)};

    sc_core::sc_join join;
    for (unsigned int i = 0; i < sizeof(forkees) / sizeof(sc_core::sc_process_handle); i++) {
      join.add_process(forkees[i]);
    }
    join.wait();

    UVM_INFO(get_name(), "Finishing sequence", uvm::UVM_FULL);
  };

  void post_body() override {
    auto starting_phase = get_starting_phase();
    if (starting_phase != nullptr) {
      starting_phase->drop_objection(this);
    }
  };

 private:
  rom_sequence* m_rom_sequence;
  ram_sequence* m_ram_sequence;
  ram_sequencer* m_ram_sequencer;
  rom_sequencer* m_rom_sequencer;
};

}  // namespace uv

#endif  // SEQUENCE_HPP
