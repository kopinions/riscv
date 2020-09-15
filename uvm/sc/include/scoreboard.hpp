#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include <tlm>
#include <uvm>
#include <warnings.hpp>

#include "packet.hpp"

namespace uv {
class analysis_fifo : public tlm::tlm_analysis_if<packet>, public tlm::tlm_fifo<packet> {
 public:
  analysis_fifo(const char* nm) : tlm::tlm_fifo<packet>(nm, -16) {}
  void write(const packet& t) override { nb_put(t); }
};

class scoreboard : public uvm::uvm_scoreboard {
 public:
  DISABLE_WARNING_PUSH
  DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
  UVM_COMPONENT_UTILS(uv::scoreboard)
  DISABLE_WARNING_POP
  friend class testbench;

  void connect_phase(uvm::uvm_phase& phase) override {
    uvm::uvm_scoreboard::connect_phase(phase);
    m_ram_analysis_export.connect(m_ram_fifo);
    m_rom_analysis_export.connect(m_rom_fifo);
    m_golden_analysis_export.connect(m_golden_fifo);
  }

  scoreboard() : scoreboard{"scoreboard"} {}

  explicit scoreboard(const uvm::uvm_component_name& name)
      : uvm::uvm_scoreboard{name},
        m_rom_analysis_export{"rom_analysis_export"},
        m_ram_analysis_export{"ram_analysis_export"},
        m_golden_analysis_export{"golden_analysis_export"},
        m_error{false},
        m_rom_fifo{"rom_fifo"},
        m_ram_fifo{"ram_fifo"},
        m_golden_fifo{"golden_fifo"} {};

  scoreboard(scoreboard&&) = delete;

  scoreboard(const scoreboard&) = delete;

  scoreboard& operator=(scoreboard&&) = delete;

  scoreboard& operator=(const scoreboard&) = delete;

  ~scoreboard() override{};

  void run_phase(uvm::uvm_phase& phase) override {
    uvm::uvm_scoreboard::run_phase(phase);
    while (true) {
      packet* ram_packet;
      packet* rom_packet;
      packet* golden_packet;
      *ram_packet = m_ram_fifo.get(nullptr);
      *rom_packet = m_rom_fifo.get(nullptr);
      *golden_packet = m_golden_fifo.get(nullptr);

      if (!ram_packet->compare(*golden_packet)) {
        m_error = true;

        ram_packet->set_name("ram_packet");
        rom_packet->set_name("rom_packet");
        golden_packet->set_name("golden_packet");
      }
    }
  }

  bool passed() { return !m_error; }

 protected:
  bool m_error;
  uvm::uvm_analysis_export<uv::packet> m_ram_analysis_export;
  uvm::uvm_analysis_export<uv::packet> m_rom_analysis_export;
  uvm::uvm_analysis_export<uv::packet> m_golden_analysis_export;
  analysis_fifo m_ram_fifo;
  analysis_fifo m_rom_fifo;
  analysis_fifo m_golden_fifo;
};
}  // namespace uv

#endif  // SCOREBOARD_HPP
