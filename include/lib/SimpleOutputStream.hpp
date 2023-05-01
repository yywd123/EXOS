#pragma once

#include <utils.hpp>
#include <lib/OutputStream.hpp>

namespace EXOS::Utils {
  class SimpleOutputStream : public _OutputStream {
  protected:
    Function<void, uint8_t> writeByte;
  public:
    SimpleOutputStream(Function<void, uint8_t> writeMethod) {
      this->writeByte = writeMethod;
    }

    void write(const uint8_t byte) {
      this->writeByte(byte);
    }

    void write(Pointer<const uint8_t> buf, size_t n) {
      iter(n) this->writeByte(((Pointer<uint8_t>)buf)[i]);
    }
  };
}