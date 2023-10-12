#include <exos/drivers.hpp>
#include <exos/pci.hpp>
#include <exos/logger.hpp>

USE(EXOS::Drivers);
USE(EXOS::Utils);

static bool
initialize(void *data) {
	Pci::PciDevice *device = (Pci::PciDevice *)data;
	if(device->classCode == 0x4 && device->subClass == 0x3 && device->vendor == 0x8086) {
		Logger::log(Logger::INFO, "detected intel hda device");
		return true;
	}
	return false;
}

DECL_DRIVER(hda){
		.magic = DRIVER_MAGIC,
		.type = DriverType::Pci,
		.initialize = initialize};