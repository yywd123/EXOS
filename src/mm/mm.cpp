#include <exos/mm.hpp>
#include <efi/efi.hpp>
#include <exos/logger.hpp>
#include <exos/panic.hpp>

USE(EXOS::Utils);

typedef struct {
	uintptr_t start;
	size_t size;
} AllocateableMemoryArea;

__NAMESPACE_DECL(Memory)

static bool
growBuffer(Status *status, void **buffer, uint64_t bufferSize) {
	bool tryAgain = false;

	if(!*buffer && bufferSize) {
		*status = EFI_BUFFER_TOO_SMALL;
	}

	if(*status == EFI_BUFFER_TOO_SMALL) {
		if(*buffer) {
			eficall(gBS->FreePool, *buffer);
		}

		eficall(gBS->AllocatePool, EfiBootServicesData, bufferSize, buffer);

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

static uint16_t
efiGetMmap() {
	Status status;

	EfiMemoryDescriptor *buffer = nullptr;

	uint64_t orignalEntryCount = 0;
	uint64_t mapKey = 0;
	uint64_t descriptorSize = 0;
	uint32_t descriptorVersion = 0;
	uint64_t bufferSize = 0;

	status = EFI_SUCCESS;
	bufferSize = sizeof(EfiMemoryDescriptor);

	while(growBuffer(&status, (void **)&buffer, bufferSize)) {
		status = eficall(gBS->GetMemoryMap, &bufferSize, buffer, &mapKey, &descriptorSize, &descriptorVersion);
	}

	if(!EFI_ERROR(status)) {
		orignalEntryCount = bufferSize / descriptorSize;
	} else {
		panic("can not get memory map");
	}

	uint16_t lastIndex = 0, mergedEntryCount = 0;
	for(uint16_t i = 0; i < orignalEntryCount; ++i) {
		if(buffer[i].type == EfiBootServicesCode || buffer[i].type == EfiBootServicesData)
			buffer[i].type = EfiConventionalMemory;
		if(i != lastIndex) {
			if(buffer[i].type == buffer[lastIndex].type) {
				buffer[lastIndex].pageCount += buffer[i].pageCount;
				continue;
			} else {
				__builtin_memcpy(&buffer[++lastIndex], &buffer[i], sizeof(EfiMemoryDescriptor));
				++mergedEntryCount;
				continue;
			}
		}
		lastIndex = i;
		++mergedEntryCount;
	}

	__iter(mergedEntryCount) {
		Logger::log(Logger::INFO, "memory area @: start 0x@, size @ pages, type @", i, buffer[i].physicalStart, (int64_t)buffer[i].pageCount, buffer[i].type);
	}

	return mergedEntryCount;
}

void
initialize() {
	uint16_t efiMmapEntryCount = efiGetMmap();
}

void *
__allocate(size_t size) {
	return nullptr;
}

__NAMESPACE_END