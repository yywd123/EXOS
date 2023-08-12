#include <mm/mm.hpp>
#include <mm/paging.hpp>
#include <efi/efi.hpp>
#include <exos/logger.hpp>
#include <exos/panic.hpp>

USE(EXOS::Utils);

__NAMESPACE_DECL(Memory)

static bool __INIT
growBuffer(Status *status, void **buffer, uint64_t bufferSize) {
	bool tryAgain = false;

	if(!*buffer && bufferSize) {
		*status = EFI_BUFFER_TOO_SMALL;
	}

	if(*status == EFI_BUFFER_TOO_SMALL) {
		if(*buffer) {
			eficall(gBS->FreePool, *buffer);
		}

		eficall(gBS->AllocatePool, EFI::BootServicesData, bufferSize, buffer);

		if(*buffer) {
			tryAgain = true;
		} else {
			*status = EFI_OUT_OF_RESOURCES;
		}
	}

	if(!tryAgain && EFI_ERROR(*status) && *buffer) {
		eficall(gBS->FreePool, *buffer);
		*buffer = NULL;
	}

	return tryAgain;
}

static uint16_t __INIT
getEfiMMap(EFI::MemoryDescriptor **buffer) {
	Status status;

	uint64_t orignalEntryCount = 0;
	uint64_t mapKey = 0;
	uint64_t descriptorSize = 0;
	uint32_t descriptorVersion = 0;
	uint64_t bufferSize = 0;

	status = EFI_SUCCESS;
	bufferSize = sizeof(EFI::MemoryDescriptor);

	while(growBuffer(&status, (void **)buffer, bufferSize)) {
		status = eficall(gBS->GetMemoryMap, &bufferSize, *buffer, &mapKey, &descriptorSize, &descriptorVersion);
	}

	if(!EFI_ERROR(status)) {
		orignalEntryCount = bufferSize / descriptorSize;
	} else {
		panic("can not get memory map");
	}

	uint16_t lastIndex = 0, mergedEntryCount = 0;
	for(uint16_t i = 0; i < orignalEntryCount; ++i) {
		if((*buffer)[i].type == EFI::BootServicesCode || (*buffer)[i].type == EFI::BootServicesData)
			(*buffer)[i].type = EFI::ConventionalMemory;
		if(i != lastIndex) {
			if((*buffer)[i].type == (*buffer)[lastIndex].type) {
				(*buffer)[lastIndex].pageCount += (*buffer)[i].pageCount;
				continue;
			} else {
				__builtin_memcpy(&(*buffer)[++lastIndex], &(*buffer)[i], sizeof(EFI::MemoryDescriptor));
				++mergedEntryCount;
				continue;
			}
		}
		lastIndex = i;
		++mergedEntryCount;
	}

	return mergedEntryCount;
}

void __INIT
initialize() {
	Paging::initialize();

	EFI::MemoryDescriptor *efimmap = nullptr;
	uint16_t efiMmapEntryCount = getEfiMMap(&efimmap);

	uint64_t totalPageCount = 0;
	_iter(efiMmapEntryCount) {
		Logger::log(Logger::DEBUG, "memory area @: start 0x@, size @ pages, type @, attr @", i, efimmap[i].physicalStart, (int64_t)efimmap[i].pageCount, efimmap[i].type, efimmap[i].attribute);
		totalPageCount += efimmap[i].pageCount;
	}
	Logger::log(Logger::DEBUG, "total @ pages(about @ gb)", (int64_t)totalPageCount, (int64_t)totalPageCount / 256 / 1024);
}

void *
__allocate(size_t size) {
	return nullptr;
}

__NAMESPACE_END