#include <exos/drivers.hpp>
#include <exos/logger.hpp>

USE(EXOS::Utils);

__NAMESPACE_DECL(Drivers)

static Driver *list = nullptr;
static uint32_t listLength = 0;

void
initializeManager() {
	getAddressFromSymbol(list, "_builtin_drvlist_start");
	uintptr_t end = 0;
	getAddressFromSymbol(end, "_builtin_drvlist_end");
	listLength = (end - (uintptr_t)list) / sizeof(Driver);
	uintptr_t kernelBase = 0;
	getAddressFromSymbol(kernelBase, "_imageBase");
	_iter(listLength) {
		if(list[i].magic == DRIVER_MAGIC) {
			if(list[i].initialize) list[i].initialize = (Fn<bool, void *>)((uintptr_t)list[i].initialize + kernelBase);
			if(list[i].enable) list[i].enable = (Fn<bool>)((uintptr_t)list[i].enable + kernelBase);
			if(list[i].disable) list[i].disable = (Fn<bool>)((uintptr_t)list[i].disable + kernelBase);
			if(list[i].uninstall) list[i].uninstall = (Fn<bool>)((uintptr_t)list[i].uninstall + kernelBase);
		}
	}
}

void
getDrivers(DriverType type, std::vector<Driver *> *out) {
	_iter(listLength) {
		if(list[i].magic == DRIVER_MAGIC && list[i].type == type)
			out->push_back(&list[i]);
	}
}

__NAMESPACE_END