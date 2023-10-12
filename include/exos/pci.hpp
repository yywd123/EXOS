#include <platform/platform.hpp>

#pragma once

__NAMESPACE_DECL(Drivers::Pci)

typedef struct PciDevice {
	uint16_t vendor;
	uint16_t device;
	uint8_t classCode;
	uint8_t subClass;
	uint16_t pad;
	union {
		uint32_t raw;
		struct {
			uint8_t reserved0;
			uint8_t func : 3;
			uint8_t slot : 5;
			uint8_t bus;
			uint8_t reserved1;
		};
	} info;

	PciDevice(uint8_t bus_, uint8_t slot_, uint8_t func_) {
		this->info.bus = bus_;
		this->info.slot = slot_;
		this->info.func = func_;
		this->isDeviceExist();	//	调用下 当更新了
	}

	bool
	isDeviceExist() {
		if((this->vendor = this->inw(0)) != 0xffff) {
			this->device = this->inw(2);
			uint16_t tmp = this->inw(10);
			this->subClass = tmp & 0xff;
			this->classCode = tmp >> 8;
			return true;
		}
		return false;
	}

	uint32_t
	getAddress(uint8_t reg) {
		return BIT(31) | this->info.raw | (reg & ~0b11);
	}

	void
	outb(uint8_t reg, uint8_t value) {
		Platform::IO::outl(0xcf8, this->getAddress(reg));
		Platform::IO::outb(0xcfc + (reg & 0b11), value);
	}

	void
	outw(uint8_t reg, uint16_t value) {
		Platform::IO::outl(0xcf8, this->getAddress(reg));
		Platform::IO::outw(0xcfc + (reg & 0b10), value);
	}

	void
	outl(uint8_t reg, uint32_t value) {
		Platform::IO::outl(0xcf8, this->getAddress(reg));
		Platform::IO::outl(0xcfc, value);
	}

	uint8_t
	inb(uint8_t reg) {
		Platform::IO::outl(0xcf8, this->getAddress(reg));
		return Platform::IO::inb(0xcfc + (reg & 0b11));
	}

	uint16_t
	inw(uint8_t reg) {
		Platform::IO::outl(0xcf8, this->getAddress(reg));
		return Platform::IO::inw(0xcfc + (reg & 0b10));
	}

	uint32_t
	inl(uint8_t reg) {
		Platform::IO::outl(0xcf8, this->getAddress(reg));
		return Platform::IO::inl(0xcfc);
	}

	bool
	operator==(const PciDevice &device) const {
		return device.info.raw == this->info.raw;
	}
} PciDevice;

void
scanPciBus(uint8_t bus);

void
initializeDevices();

__NAMESPACE_END
