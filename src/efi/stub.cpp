#include <efi/efi.hpp>
#include <exos/efifb.hpp>
#include <exos/fbcon.hpp>
#include <exos/serial.hpp>

USE(EXOS::Drivers);

void __INIT
initializeKernel();

extern "C" uint64_t __INIT
efiEntry(Handle imageHandle, EFI::SystemTable *systemTable) {
	eficall(systemTable->bootServices->SetWatchdogTimer, 0, 0, 0, nullptr);
	EFI::initialize(imageHandle, systemTable);
	EFI::clearScreen();

	Serial::initialize();

	EfiFb::initialize();
	FbConsole::initialize();

	initializeKernel();

	while(1)
		;

	return 0;
}
