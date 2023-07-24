#include <platform/platform.hpp>
#include <efi/efi.hpp>
#include <mm/mm.hpp>
#include <exos/acpi.hpp>
#include <exos/cmos.hpp>
#include <exos/keyboard.hpp>
#include <exos/hpet.hpp>

USE(EXOS::Drivers);
USE(EXOS::Utils);

__NAMESPACE_DECL(Platform)

void __INIT
initialize() {
	Memory::initialize();
	CMOS::setTimeOffset(8, 0);	//	CST = UTC+8

	EFI::SystemTable *systemTable = EFI::getSystemTable();

	void *acpiRsdptr = nullptr;
	void *acpi2Rsdptr = nullptr;

#define EntryMatcher(_guid, _table)                                 \
	if(guidCompare(systemTable->configurationTable[i].guid, _guid)) { \
		_table = systemTable->configurationTable[i].table;              \
		continue;                                                       \
	}
	__iter(systemTable->tableEntryCount) {
		EntryMatcher("eb9d2d30-2d88-11d3-9a16-0090273fc14d", acpiRsdptr);
		EntryMatcher("8868e871-e4f1-11d3-bc22-0080c73c8881", acpi2Rsdptr);
	}
#undef EntryMatcher

	if(acpi2Rsdptr) acpiRsdptr = acpi2Rsdptr;
	Acpi::initialize((Acpi::Rsdp *)acpiRsdptr);
	MultiProcessor::initialize();
	HPET::initialize();
	CMOS::initialize();
	Keyboard::initialize();
}

__NAMESPACE_END