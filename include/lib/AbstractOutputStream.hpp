#pragma once

#include <utils.hpp>

namespace EXOS::Utils {
  template<typename T>
  class AbstractOutputOutputStream {
  public:
    virtual void write(T t) = 0;
    virtual void write(Pointer<T> buf, size_t n) = 0;
  };
}