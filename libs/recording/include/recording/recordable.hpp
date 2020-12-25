#pragma once
#include <tlm>

#include "recording/recorder.hpp"

class recordable {
 public:
  virtual bool enabled() const = 0;
  virtual ~recordable() = default;
};
