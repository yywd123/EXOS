#pragma once

#include <utils.hpp>
#include <lib/OutputStream.hpp>
#include <driver/Serial.hpp>

namespace EXOS::Utils {
  class SerialOutputStream : public _OutputStream {
    EXOS::Driver::Serial::SerialPort port;
  public:
    SerialOutputStream(EXOS::Driver::Serial::SerialPort port) {
      this->port = port;      
    }

    void write(const uint8_t byte) {
      EXOS::Driver::Serial::write(this->port, byte);
    }

    void write(Pointer<const void> buf, size_t n) {
      iter(n) EXOS::Driver::Serial::write(this->port, ((Pointer<uint8_t>)buf)[i]);
    }
  };
}