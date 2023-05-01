#pragma once

#include <lib/AbstractOutputStream.hpp>

namespace EXOS::Utils {
  class _OutputStream : public AbstractOutputOutputStream<const uint8_t> {};

  using OutputStream = _OutputStream*;
}