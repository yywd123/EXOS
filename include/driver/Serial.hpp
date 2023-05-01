#pragma once

#include <utils.hpp>

namespace EXOS::Driver::Serial {
  typedef enum {
    COM1,
    COM2,
    COM3,
    COM4,
    COM5,
    COM6,
    COM7,
    COM8
  } SerialPort;

  void write(SerialPort port, uint8_t byte);
  uint8_t read(SerialPort port);
}