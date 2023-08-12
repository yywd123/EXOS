#include <efi/efi.hpp>
#include <exos/efifb.hpp>
#include <exos/logger.hpp>

USE(EXOS::Drivers);
USE(EXOS::Utils);

void __INIT
initializeKernel();

extern "C" uint64_t __INIT
efiEntry(Handle imageHandle, EFI::SystemTable *systemTable) {
	eficall(systemTable->bootServices->SetWatchdogTimer, 0, 0, 0, nullptr);
	EFI::initialize(imageHandle, systemTable);

	Serial::initialize();

	uintptr_t kernelBase = 0;
	getAddressFromSymbol(kernelBase, "_imageBase");
	Logger::log(Logger::DEBUG, "kernel base 0x@", kernelBase);

	initializeKernel();

	while(1)
		;

	return 0;
}
