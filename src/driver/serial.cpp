#include <exos/serial.hpp>
#include <platform/platform.hpp>

USE(EXOS::Platform::IO);

__NAMESPACE_DECL(Drivers::Serial)

static SerialStatus portStatus[8] = {0};

static inline uint16_t
getPort(SerialPort port) {
	switch(port) {
	default:
	case 0:
		return 0x3f8;
	case 1:
		return 0x2f8;
	case 2:
		return 0x3e8;
	case 3:
		return 0x2e8;
	case 4:
		return 0x5f8;
	case 5:
		return 0x4f8;
	case 6:
		return 0x5e8;
	case 7:
		return 0x4e8;
	}
}

void __INIT
initialize() {
	SerialPort ports[8] = {COM1, COM2, COM3, COM4, COM5, COM6, COM7, COM8};

	__iter(8) {
		uint16_t port = getPort(ports[i]);

		outb(port + 1, 0);
		outb(port + 3, BIT(7));
		outb(port + 0, 0x01);	 // 0x  01 = 115200 / 0x0001 = 115200bps
		outb(port + 1, 0x00);	 //   00
		outb(port + 3, 0b00000011);
		outb(port + 2, 0b11000111);
		outb(port + 4, 0b00001111);
		outb(port + 4, 0b00011110);

		//  发送校验码并检查
		outb(port, 0x58);
		if(inb(port) != 0x58) continue;
		portStatus[i].initialized = true;

		outb(port + 4, 0b00001111);
	}
}

void
write(SerialPort port, uint8_t byte) {
	if(!portStatus[port].initialized) return;

	while((inb(getPort(port) + 5) & BIT(5)) == 0)
		;
	outb(getPort(port), byte);
}

uint8_t
read(SerialPort port) {
	if(!portStatus[port].initialized) return 0;

	while((inb(getPort(port) + 5) & BIT(0)) == 0)
		;
	return inb(getPort(port));
}

SerialStatus
getStatus(SerialPort port) {
	return portStatus[port];
}

__NAMESPACE_END
