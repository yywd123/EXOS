#include <efi/efi.hpp>

static Handle gImageHandle __INITDATA;
static EfiSystemTable *gSystemTable __INITDATA;

void __INIT
initializeEfiUtils(Handle imageHandle, EfiSystemTable *systemTable) {
	gImageHandle = imageHandle;
	gSystemTable = systemTable;
}

Handle __INIT
efiGetImageHandle() {
	return gImageHandle;
}

EfiSystemTable *__INIT
efiGetSystemTable() {
	return gSystemTable;
}

void __INIT
efiClearScreen() {
	eficall(gSystemTable->ConOut->ClearScreen, gSystemTable->ConOut);
}

void __INIT
efiPuts(const wchar_t *s) {
	eficall(gSystemTable->ConOut->OutputString, gSystemTable->ConOut, s);
}

void __INIT
efiExit(uint64_t status) {
	eficall(gSystemTable->BootServices->Exit, status, 0, 0);
}

Handle __INIT
efiLocateProtocol(UUID *guid, Handle registration) {
	Handle protocolInterface = nullptr;
	eficall(gSystemTable->BootServices->LocateProtocol, guid, registration, &protocolInterface);
	return protocolInterface;
}

void *__INIT
efiAllocatePool(size_t size) {
	void *pool = nullptr;
	eficall(gSystemTable->BootServices->AllocatePool, EfiRuntimeServicesData, size, &pool);
	return pool;
}

void __INIT
efiFreePool(void *pool) {
	eficall(gSystemTable->BootServices->FreePool, pool);
}

void *__INIT
efiAllocatePages(uint64_t count) {
	void *page = nullptr;
	eficall(gSystemTable->BootServices->AllocatePages, AllocateAnyPages, EfiRuntimeServicesData, count, &page);
	return page;
}

void __INIT
efiFreePages(void *page, uint64_t count) {
	eficall(gSystemTable->BootServices->FreePages, page, count);
}

static EfiDevicePath *
getRootFsDevicePath(Handle imageHandle) {
	EFI_LOADED_IMAGE_PROTOCOL *loadedImageProtocol;
	EfiDevicePath *rootFsDevicePath;
	UUID loadedImageGuid = EFI_LOADED_IMAGE_DEVICE_PATH_PROTOCOL_GUID;
	UUID devPathGuid = EFI_DEVICE_PATH_PROTOCOL_GUID;
	eficall(
			gSystemTable->BootServices->OpenProtocol,
			imageHandle,
			&loadedImageGuid,
			(void **)&loadedImageProtocol,
			imageHandle,
			nullptr,
			EFI_OPEN_PROTOCOL_GET_PROTOCOL);

	eficall(
			gSystemTable->BootServices->OpenProtocol,
			loadedImageProtocol->DeviceHandle,
			&devPathGuid,
			(void **)&rootFsDevicePath,
			imageHandle,
			nullptr,
			EFI_OPEN_PROTOCOL_GET_PROTOCOL);

	return rootFsDevicePath;
}

EFI_FILE_HANDLE
efiOpenRootFs() {
	UUID fsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
	EFI_FILE_IO_INTERFACE *rootVolume = (EFI_FILE_IO_INTERFACE *)efiLocateProtocol(&fsGuid, nullptr);
	EFI_FILE_HANDLE rootDir;
	eficall(rootVolume->OpenVolume, rootVolume, &rootDir);
	return rootDir;
}

static uint16_t
getFileNameLength(const wchar_t *filePath) {
	uint16_t length = 0, i = 0;
	while(filePath[i] != 0) {
		if(filePath[i] == L'\\') {
			length = 0;
			++i;
			continue;
		}
		++length;
		++i;
	}
	return length;
}

uint64_t
efiReadFile(EFI_FILE_HANDLE fs, const wchar_t *filePath, uintptr_t *address, EFI_MEMORY_TYPE memoryType, uint64_t fileOffset, uint64_t readSize) {
	Status status = EFI_SUCCESS;
	EFI_FILE_HANDLE file;
	status = eficall(fs->Open, fs, &file, filePath, EFI_FILE_MODE_READ, 0);

	if(EFI_ERROR(status)) {
		return status;
	}
	EFI_FILE_INFO *fileInfo;
	uint64_t infoSize = sizeof(EFI_FILE_INFO) + (getFileNameLength(filePath) + 1) * sizeof(wchar_t);
	eficall(gSystemTable->BootServices->AllocatePool, EfiLoaderData, infoSize, (void **)&fileInfo);
	UUID fileInfoGuid = EFI_FILE_INFO_ID;
	eficall(file->GetInfo, file, &fileInfoGuid, &infoSize, fileInfo);

	if(fileOffset + readSize > fileInfo->FileSize) return EFI_INVALID_PARAMETER;

	uint64_t pageCount = (readSize == 0 ? fileInfo->FileSize : readSize) / EFI_PAGE_SIZE + 1;

	eficall(
			gSystemTable->BootServices->AllocatePages,
			AllocateAddress,	//*address == 0 ? AllocateAnyPages : AllocateAddress,
			memoryType,
			pageCount,
			address);
	if(fileOffset != 0) eficall(file->SetPosition, file, fileOffset);

	eficall(file->Read, file, (readSize == 0 ? &fileInfo->FileSize : &readSize), (void *)*address);

	eficall(file->Close, file);
	eficall(gSystemTable->BootServices->FreePool, fileInfo);

	return pageCount;
}

static bool
growBuffer(Status *status, void **buffer, uint64_t bufferSize) {
	bool tryAgain = false;

	if(!*buffer && bufferSize) {
		*status = EFI_BUFFER_TOO_SMALL;
	}

	if(*status == EFI_BUFFER_TOO_SMALL) {
		if(*buffer) {
			eficall(gSystemTable->BootServices->FreePool, *buffer);
		}

		eficall(gSystemTable->BootServices->AllocatePool, EfiLoaderData, bufferSize, buffer);

		if(*buffer) {
			tryAgain = true;
		} else {
			*status = EFI_OUT_OF_RESOURCES;
		}
	}

	if(!tryAgain && EFI_ERROR(*status) && *buffer) {
		eficall(gSystemTable->BootServices->FreePool, *buffer);
		*buffer = NULL;
	}

	return tryAgain;
}

static EfiMemoryDescriptor *mmap = nullptr;
static uint64_t entryCount;
static uint64_t mapKey;
static uint64_t descriptorSize;
static uint32_t descriptorVersion;

void __INIT
efiExitBootServices() {
	Status status = EFI_SUCCESS;
	uint64_t bufferSize = sizeof(EfiMemoryDescriptor);

	while(growBuffer(&status, (void **)&mmap, bufferSize)) {
		status = eficall(gSystemTable->BootServices->GetMemoryMap, &bufferSize, mmap, &mapKey, &descriptorSize, &descriptorVersion);
	}

	if(!EFI_ERROR(status)) {
		entryCount = bufferSize / descriptorSize;
	} else {
		efiPuts(L"\n!!!cannot exit BootServices, halting!!!\n");
		ASM("hlt");
	}

	eficall(gSystemTable->BootServices->ExitBootServices, gImageHandle, mapKey);
}
