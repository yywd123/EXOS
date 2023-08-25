#include <mm/mm.hpp>
#include <mm/paging.hpp>
#include <mm/kmalloc.hpp>
#include <efi/efi.hpp>
#include <exos/logger.hpp>
#include <exos/panic.hpp>

USE(EXOS::Utils);

__NAMESPACE_DECL(Memory)

void __INIT
initialize() {
	KMemory::initialize();
	Paging::initialize();
}

__NAMESPACE_END