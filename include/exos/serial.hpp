#pragma once

__NAMESPACE_DECL(Drivers::Serial)

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

typedef union {
  uint8_t container;
  struct {
    bool initialized : 1;
    bool unused : 7;
  } __packed;
} SerialStatus;

void __INIT
initializeSerialPorts();

void
write(SerialPort port, uint8_t byte);

uint8_t
read(SerialPort port);

SerialStatus
getStatus(SerialPort port);

__NAMESPACE_END