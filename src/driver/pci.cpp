#include <exos/pci.hpp>
#include <exos/logger.hpp>
#include <exos/drivers.hpp>
#include <algorithm>

USE(EXOS::Platform);
USE(EXOS::Utils);

__NAMESPACE_DECL(Drivers::Pci)

static std::vector<PciDevice> *pciDevices = nullptr;

void
printDeviceType(PciDevice device) {
	if(device.classCode == 0x0) {
		Logger::print("Unclassified: ");
		if(device.subClass == 0x0) {
			Logger::print("Other Device");
			return;
		}
		if(device.subClass == 0x1) {
			Logger::print("VGA-Compatible Device");
			return;
		}
	}
	if(device.classCode == 0x1) {
		Logger::print("Mass Storage Controller: ");
		if(device.subClass == 0x0) {
			Logger::print("SCSI Bus Controller");
			return;
		}
		if(device.subClass == 0x1) {
			Logger::print("IDE Controller");
			return;
		}
		if(device.subClass == 0x2) {
			Logger::print("Floppy Disk Controller");
			return;
		}
		if(device.subClass == 0x3) {
			Logger::print("IPI Bus Controller");
			return;
		}
		if(device.subClass == 0x4) {
			Logger::print("RAID Controller");
			return;
		}
		if(device.subClass == 0x5) {
			Logger::print("ATA Controller");
			return;
		}
		if(device.subClass == 0x6) {
			Logger::print("Serial ATA Controller");
			return;
		}
		if(device.subClass == 0x7) {
			Logger::print("Serial Attached SCSI Controller");
			return;
		}
		if(device.subClass == 0x8) {
			Logger::print("Non-Volatile Memory Controller");
			return;
		}
	}
	if(device.classCode == 0x2) {
		Logger::print("Network Controller: ");
		if(device.subClass == 0x0) {
			Logger::print("Ethernet Controller");
			return;
		}
		if(device.subClass == 0x1) {
			Logger::print("Token Ring Controller");
			return;
		}
		if(device.subClass == 0x2) {
			Logger::print("FDDI Controller");
			return;
		}
		if(device.subClass == 0x3) {
			Logger::print("ATM Controller");
			return;
		}
		if(device.subClass == 0x4) {
			Logger::print("ISDN Controller");
			return;
		}
		if(device.subClass == 0x5) {
			Logger::print("WorldFip Controller");
			return;
		}
		if(device.subClass == 0x6) {
			Logger::print("PICMG 2.14 Multi Computing Controller");
			return;
		}
		if(device.subClass == 0x7) {
			Logger::print("Infiniband Controller");
			return;
		}
		if(device.subClass == 0x8) {
			Logger::print("Fabric Controller");
			return;
		}
	}
	if(device.classCode == 0x3) {
		Logger::print("Display Controller: ");
		if(device.subClass == 0x0) {
			Logger::print("VGA Compatible Controller");
			return;
		}
		if(device.subClass == 0x1) {
			Logger::print("XGA Controller");
			return;
		}
		if(device.subClass == 0x2) {
			Logger::print("3D Controller (Not VGA-Compatible)");
			return;
		}
	}
	if(device.classCode == 0x4) {
		Logger::print("Multimedia Controller: ");
		if(device.subClass == 0x0) {
			Logger::print("Multimedia Video Controller");
			return;
		}
		if(device.subClass == 0x1) {
			Logger::print("Multimedia Audio Controller");
			return;
		}
		if(device.subClass == 0x2) {
			Logger::print("Computer Telephony Device");
			return;
		}
		if(device.subClass == 0x3) {
			Logger::print("Audio Device");
			return;
		}
	}
	if(device.classCode == 0x5) {
		Logger::print("Memory Controller: ");
		if(device.subClass == 0x0) {
			Logger::print("RAM Controller");
			return;
		}
		if(device.subClass == 0x1) {
			Logger::print("Flash Controller");
			return;
		}
	}
	if(device.classCode == 0x6) {
		Logger::print("Bridge: ");
		if(device.subClass == 0x0) {
			Logger::print("Host Bridge");
			return;
		}
		if(device.subClass == 0x1) {
			Logger::print("ISA Bridge");
			return;
		}
		if(device.subClass == 0x2) {
			Logger::print("EISA Bridge");
			return;
		}
		if(device.subClass == 0x3) {
			Logger::print("MCA Bridge");
			return;
		}
		if(device.subClass == 0x4 || device.subClass == 0x9) {
			Logger::printf("PCI-to-PCI Bridge(primary @, secondary @)", (index_t)device.inb(0x18), (index_t)device.inb(0x19));
			return;
		}
		if(device.subClass == 0x5) {
			Logger::print("PCMCIA Bridge");
			return;
		}
		if(device.subClass == 0x6) {
			Logger::print("NuBus Bridge");
			return;
		}
		if(device.subClass == 0x7) {
			Logger::print("CardBus Bridge");
			return;
		}
		if(device.subClass == 0x8) {
			Logger::print("RACEway Bridge");
			return;
		}
		if(device.subClass == 0xa) {
			Logger::print("InfiniBand-to-PCI Host Bridge");
			return;
		}
	}
	if(device.classCode == 0x7) {
		Logger::print("Simple Communication Controller: ");
		if(device.subClass == 0x0) {
			Logger::print("Serial Controller");
			return;
		}
		if(device.subClass == 0x1) {
			Logger::print("Parallel Controller");
			return;
		}
		if(device.subClass == 0x2) {
			Logger::print("Multiport Serial Controller");
			return;
		}
		if(device.subClass == 0x3) {
			Logger::print("Modem");
			return;
		}
		if(device.subClass == 0x4) {
			Logger::print("IEEE 488.1/2 (GPIB) Controller");
			return;
		}
		if(device.subClass == 0x5) {
			Logger::print("Smart Card Controller");
			return;
		}
	}
	if(device.classCode == 0x8) {
		Logger::print("Base System Peripheral: ");
		if(device.subClass == 0x0) {
			Logger::print("PIC");
			return;
		}
		if(device.subClass == 0x1) {
			Logger::print("DMA Controller");
			return;
		}
		if(device.subClass == 0x2) {
			Logger::print("Timer");
			return;
		}
		if(device.subClass == 0x3) {
			Logger::print("RTC Controller");
			return;
		}
		if(device.subClass == 0x4) {
			Logger::print("PCI Hot-Plug Controller");
			return;
		}
		if(device.subClass == 0x5) {
			Logger::print("SD Host Controller");
			return;
		}
		if(device.subClass == 0x6) {
			Logger::print("IOMMU");
			return;
		}
	}
	if(device.classCode == 0x9) {
		Logger::print("Input Device Controller: ");
		if(device.subClass == 0x0) {
			Logger::print("Keyboard Controller");
			return;
		}
		if(device.subClass == 0x1) {
			Logger::print("Digitizer Pen");
			return;
		}
		if(device.subClass == 0x2) {
			Logger::print("Mouse Controller");
			return;
		}
		if(device.subClass == 0x3) {
			Logger::print("Scanner Controller");
			return;
		}
		if(device.subClass == 0x4) {
			Logger::print("Gameport Controller");
			return;
		}
		return;
	}
	if(device.classCode == 0xa) {
		Logger::print("Docking Station: ");
		if(device.subClass == 0x0) {
			Logger::print("Generic");
			return;
		}
	}
	if(device.classCode == 0xb) {
		Logger::print("Processor: ");
		if(device.subClass == 0x0) {
			Logger::print("386");
			return;
		}
		if(device.subClass == 0x1) {
			Logger::print("486");
			return;
		}
		if(device.subClass == 0x2) {
			Logger::print("Pentium");
			return;
		}
		if(device.subClass == 0x3) {
			Logger::print("Pentium Pro");
			return;
		}
		if(device.subClass == 0x10) {
			Logger::print("Alpha");
			return;
		}
		if(device.subClass == 0x20) {
			Logger::print("PowerPC");
			return;
		}
		if(device.subClass == 0x30) {
			Logger::print("MIPS");
			return;
		}
		if(device.subClass == 0x40) {
			Logger::print("Co-Processor");
			return;
		}
	}
	if(device.classCode == 0xc) {
		Logger::print("Serial Bus Controller: ");
		if(device.subClass == 0x0) {
			Logger::print("FireWire (IEEE 1394) Controller");
			return;
		}
		if(device.subClass == 0x1) {
			Logger::print("ACCESS Bus Controller");
			return;
		}
		if(device.subClass == 0x2) {
			Logger::print("SSA");
			return;
		}
		if(device.subClass == 0x3) {
			Logger::print("USB Controller");
			return;
		}
		if(device.subClass == 0x4) {
			Logger::print("Fibre Channel");
			return;
		}
		if(device.subClass == 0x5) {
			Logger::print("SMBus Controller");
			return;
		}
		if(device.subClass == 0x6) {
			Logger::print("InfiniBand Controller");
			return;
		}
		if(device.subClass == 0x7) {
			Logger::print("IMPI Interface");
			return;
		}
		if(device.subClass == 0x8) {
			Logger::print("SERCOS Interface (IEC 61491)");
			return;
		}
		if(device.subClass == 0x8) {
			Logger::print("CANBus Controller");
			return;
		}
	}

	Logger::print("Unknown Device");
	if(device.vendor == 0x8086) {
		Logger::printf("(Intel Corporation Device @)", device.device);
		return;
	}
}

void
scanPciBus(uint8_t bus) {
	__iter(dev, 32) {
		__iter(func, 8) {
			PciDevice device(bus, dev, func);
			if(device.isDeviceExist()) {
				if(std::find(pciDevices->begin(), pciDevices->end(), device) == pciDevices->end()) {
					pciDevices->push_back(device);
				}
				if(device.classCode == 0x6 && (device.subClass == 0x4 || device.subClass == 0x9)) {
					scanPciBus(device.inb(0x19));
				}
			}
		}
	}
}

void
initializeDevices() {
	std::vector<Driver *> drivers;
	pciDevices = new std::vector<PciDevice>;

	getDrivers(DriverType::Pci, &drivers);
	Logger::log(Logger::INFO, "@ drivers avaliable", (int64_t)drivers.size());

	scanPciBus(0);
	for(PciDevice device : *pciDevices) {
		Logger::printf("pci @:@.@ @:@ @.@: ", device.info.bus, device.info.slot, (int8_t)device.info.func, device.vendor, device.device, device.classCode, device.subClass);
		printDeviceType(device);
		Logger::print('\n');
		for(Driver *driver : drivers) {
			if(driver->initialize) {
				if(driver->initialize(&device)) break;
			}
		}
	}
}

__NAMESPACE_END