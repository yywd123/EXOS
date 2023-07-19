#ifndef __EFIAPI_H
#define __EFIAPI_H

/*++

Copyright (c) 1998 Intel Corporation

Module Name:

	api.h

Abstract:

	Global  runtime & boot service interfaces




Revision History

--*/

//
//  Specification Revision
//

#define EFI_SPECIFICATION_MAJOR_REVISION 1
#define EFI_SPECIFICATION_MINOR_REVISION 02

//
//  Memory
//

typedef Status(EFIAPI *FnAllocatePages)(AllocateType type, MemoryType memoryType, uint64_t pageCount, uintptr_t *memory);

typedef Status(EFIAPI *FnFreePages)(uintptr_t memory, uint64_t pageCount);

typedef Status(EFIAPI *FnGetMemoryMap)(uint64_t *memoryMapSize, MemoryDescriptor *memoryMap, uint64_t *mapKey, uint64_t *descriptorSize, uint32_t *descriptorVersion);

#define NextMemoryDescriptor(Ptr, Size) ((MemoryDescriptor *)(((uint8_t *)Ptr) + Size))

typedef Status(EFIAPI *FnAllocatePool)(MemoryType poolType, uint64_t size, void **buffer);

typedef Status(EFIAPI *FnFreePool)(void *buffer);

typedef Status(EFIAPI *FnSetVituralMemoryMap)(uint64_t memoryMapSize, uint64_t descriptorSize, uint32_t descriptorVersion, MemoryDescriptor *virtualMap);

#define EFI_OPTIONAL_PTR 0x00000001
#define EFI_INTERNAL_FNC 0x00000002	 // Pointer to internal runtime fnc
#define EFI_INTERNAL_PTR 0x00000004	 // Pointer to internal runtime data

typedef Status(EFIAPI *FnConvertRuntimePointer)(uint64_t debugDisposition, void **address);

//
//  Events
//

#define EVT_TIMER 0x80000000
#define EVT_RUNTIME 0x40000000
#define EVT_RUNTIME_CONTEXT 0x20000000

#define EVT_NOTIFY_WAIT 0x00000100
#define EVT_NOTIFY_SIGNAL 0x00000200

#define EVT_SIGNAL_EXIT_BOOT_SERVICES 0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE 0x60000202

#define EVT__SIGNAL_MASK 0x000000FF
#define EVT__SIGNAL_MAX 4

#define EFI_EVENT_TIMER EVT_TIMER
#define EFI_EVENT_RUNTIME EVT_RUNTIME
#define EFI_EVENT_RUNTIME_CONTEXT EVT_RUNTIME_CONTEXT
#define EFI_EVENT_NOTIFY_WAIT EVT_NOTIFY_WAIT
#define EFI_EVENT_NOTIFY_SIGNAL EVT_NOTIFY_SIGNAL
#define EFI_EVENT_SIGNAL_EXIT_BOOT_SERVICES EVT_SIGNAL_EXIT_BOOT_SERVICES
#define EFI_EVENT_SIGNAL_VIRTUAL_ADDRESS_CHANGE EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE
#define EFI_EVENT__SIGNAL_MASK EVT__SIGNAL_MASK
#define EFI_EVENT__SIGNAL_MAX EVT__SIGNAL_MAX

typedef void(EFIAPI *FnEventNotify)(Event event, void *context);

typedef Status(EFIAPI *FnCreateEvent)(uint32_t type, Tpl notifyTpl, FnEventNotify notifyFunction, void *notifyContext, Event *event);

typedef enum {
	TimerCancel,
	TimerPeriodic,
	TimerRelative,
	TimerTypeMax
} TimerDelay;

typedef Status(EFIAPI *FnSetTimer)(Event Event, TimerDelay Type, uint64_t triggerTime);

typedef Status(EFIAPI *FnSignalEvent)(Event event);

typedef Status(EFIAPI *FnWaitForEvent)(uint64_t numberOfEvents, Event *event, uint64_t *index);

typedef Status(EFIAPI *FnCloseEvent)(Event event);

typedef Status(EFIAPI *FnCheckEvent)(Event event);

//
// Task priority level
//

#define TPL_APPLICATION 4
#define TPL_CALLBACK 8
#define TPL_NOTIFY 16
#define TPL_HIGH_LEVEL 31
#define EFI_TPL_APPLICATION TPL_APPLICATION
#define EFI_TPL_CALLBACK TPL_CALLBACK
#define EFI_TPL_NOTIFY TPL_NOTIFY
#define EFI_TPL_HIGH_LEVEL TPL_HIGH_LEVEL
typedef Tpl(EFIAPI *FnRaiseTpl)(Tpl newTpl);

typedef void(EFIAPI *FnRestoreTpl)(Tpl oldTpl);

//
//  platform varibles
//

#define EFI_GLOBAL_VARIABLE                          \
	{                                                  \
		0x8BE4DF61, 0x93CA, 0x11d2, {                    \
			0xAA, 0x0D, 0x00, 0xE0, 0x98, 0x03, 0x2B, 0x8C \
		}                                                \
	}

// Variable attributes
#define EFI_VARIABLE_NON_VOLATILE 0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS 0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS 0x00000004
#define EFI_VARIABLE_HARDWARE_ERROR_RECORD 0x00000008
#define EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS 0x00000010
#define EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS 0x00000020
#define EFI_VARIABLE_APPEND_WRITE 0x00000040

// Variable size limitation
#define EFI_MAXIMUM_VARIABLE_SIZE 1024

typedef Status(EFIAPI *FnGetVariable)(
		const unsigned short *VariableName,
		UUID *VendorGuid,
		uint32_t *Attributes OPTIONAL,
		uint64_t *DataSize,
		void *Data);

typedef Status(EFIAPI *FnGetNextVariableName)(
		uint64_t *VariableNameSize,
		const unsigned short *VariableName,
		UUID *VendorGuid);

typedef Status(EFIAPI *FnSetVariable)(
		const unsigned short *VariableName,
		UUID *VendorGuid,
		uint32_t Attributes,
		uint64_t DataSize,
		void *Data);

//
//  Time
//

typedef struct {
	uint32_t Resolution;	// 1e-6 parts per million
	uint32_t Accuracy;		// hertz
	bool SetsToZero;			// Set clears sub-second time
} EFI_TIME_CAPABILITIES;

typedef Status(EFIAPI *FnGetTime)(
		Time *Time,
		EFI_TIME_CAPABILITIES *Capabilities OPTIONAL);

typedef Status(EFIAPI *FnSetTime)(
		Time *Time);

typedef Status(EFIAPI *FnGetWakeupTime)(
		bool *Enabled,
		bool *Pending,
		Time *Time);

typedef Status(EFIAPI *FnSetWakeupTime)(
		bool Enable,
		Time *Time OPTIONAL);

//
// Image functions
//

// PE32+ Subsystem type for  images

#if !defined(IMAGE_SUBSYSTEM__APPLICATION)
#define IMAGE_SUBSYSTEM__APPLICATION 10
#define IMAGE_SUBSYSTEM__BOOT_SERVICE_DRIVER 11
#define IMAGE_SUBSYSTEM__RUNTIME_DRIVER 12
#endif

// PE32+ Machine type for  images

#if !defined(_IMAGE_MACHINE_IA32)
#define EFI_IMAGE_MACHINE_IA32 0x014c
#endif

#if !defined(_IMAGE_MACHINE_IA64)
#define EFI_IMAGE_MACHINE_IA64 0x0200
#endif

#if !defined(_IMAGE_MACHINE_EBC)
#define EFI_IMAGE_MACHINE_EBC 0x0EBC
#endif

#if !defined(_IMAGE_MACHINE_X64)
#define EFI_IMAGE_MACHINE_X64 0x8664
#endif

#if !defined(_IMAGE_MACHINE_ARMTHUMB_MIXED)
#define EFI_IMAGE_MACHINE_ARMTHUMB_MIXED 0x01C2
#endif

#if !defined(_IMAGE_MACHINE_AARCH64)
#define EFI_IMAGE_MACHINE_AARCH64 0xAA64
#endif

typedef Status(EFIAPI *FnLoadImage)(
		bool BootPolicy,
		Handle ParentImageHandle,
		DevicePath *FilePath,
		void *SourceBuffer OPTIONAL,
		uint64_t SourceSize,
		Handle *ImageHandle);

typedef Status(EFIAPI *FnStartImage)(
		Handle ImageHandle,
		uint64_t *ExitDataSize,
		const unsigned short **ExitData OPTIONAL);

typedef Status(EFIAPI *FnExit)(
		Handle ImageHandle,
		Status ExitStatus,
		uint64_t ExitDataSize,
		const unsigned short *ExitData OPTIONAL);

typedef Status(EFIAPI *FnExitBootServices)(
		Handle ImageHandle,
		uint64_t MapKey);

//
// Misc
//

typedef Status(EFIAPI *FnStall)(
		uint64_t Microseconds);

typedef Status(EFIAPI *FnSetWatchdogTimer)(
		uint64_t Timeout,
		uint64_t WatchdogCode,
		uint64_t DataSize,
		const unsigned short *WatchdogData OPTIONAL);

typedef Status(EFIAPI *FnConnectController)(
		Handle ControllerHandle,
		Handle *DriverImageHandle OPTIONAL,
		DevicePath *RemainingDevicePath OPTIONAL,
		bool Recursive);

typedef Status(EFIAPI *FnDisconnectController)(
		Handle ControllerHandle,
		Handle DriverImageHandle OPTIONAL,
		Handle ChildHandle OPTIONAL);

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL 0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL 0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL 0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER 0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE 0x00000020

typedef Status(EFIAPI *FnOpenProtocol)(
		Handle handle,
		UUID *Protocol,
		void **Interface OPTIONAL,
		Handle AgentHandle,
		Handle ControllerHandle,
		uint32_t Attributes);

typedef Status(EFIAPI *FnCloseProtocol)(
		Handle handle,
		UUID *Protocol,
		Handle AgentHandle,
		Handle ControllerHandle);

typedef struct {
	Handle AgentHandle;
	Handle ControllerHandle;
	uint32_t Attributes;
	uint32_t OpenCount;
} OpenProtocolInfomationEntry;

typedef Status(EFIAPI *FnOpenProtocolInfomation)(
		Handle handle,
		UUID *Protocol,
		OpenProtocolInfomationEntry **EntryBuffer,
		uint64_t *EntryCount);

typedef Status(EFIAPI *FnProtocolsPerHandles)(
		Handle handle,
		UUID ***ProtocolBuffer,
		uint64_t *ProtocolBufferCount);

typedef enum {
	AllHandles,
	ByRegisterNotify,
	ByProtocol
} LocateSearchType;

typedef Status(EFIAPI *FnLocateHandleBuffer)(
		LocateSearchType SearchType,
		UUID *Protocol OPTIONAL,
		void *SearchKey OPTIONAL,
		uint64_t *NoHandles,
		Handle **Buffer);

typedef Status(EFIAPI *FnLocateProtocol)(
		UUID *Protocol,
		void *Registration OPTIONAL,
		void **Interface);

typedef Status(EFIAPI *FnInstallMultipleProtocolInterfaces)(
		Handle *Handle,
		...);

typedef Status(EFIAPI *FnUninstallMultipleProtocolInterfaces)(
		Handle Handle,
		...);

typedef Status(EFIAPI *FnCalculateCrc32)(
		void *Data,
		uint64_t DataSize,
		uint32_t *Crc32);

typedef void(EFIAPI *FnMemcpy)(
		void *Destination,
		void *Source,
		uint64_t Length);

typedef void(EFIAPI *FnMemset)(
		void *Buffer,
		uint64_t Size,
		uint8_t Value);

typedef Status(EFIAPI *FnCreateEventEx)(
		uint32_t Type,
		Tpl NotifyTpl,
		FnEventNotify NotifyFunction OPTIONAL,
		const void *NotifyContext OPTIONAL,
		const UUID EventGroup OPTIONAL,
		Event *Event);

typedef enum {
	ResetCold,
	ResetWarm,
	ResetShutdown
} ResetType;

typedef Status(EFIAPI *FnResetSystem)(
		ResetType ResetType,
		Status ResetStatus,
		uint64_t DataSize,
		const unsigned short *ResetData OPTIONAL);

typedef Status(EFIAPI *FnGetNextMonotonicCount)(
		uint64_t *Count);

typedef Status(EFIAPI *FnGetNextHighMonotonicCount)(
		uint32_t *HighCount);

typedef struct {
	uint64_t Length;
	union {
		uintptr_t DataBlock;
		uintptr_t ContinuationPointer;
	} Union;
} CapsuleBlockDescriptor;

typedef struct {
	UUID CapsuleGuid;
	uint32_t HeaderSize;
	uint32_t Flags;
	uint32_t CapsuleImageSize;
} CapsuleHeader;

#define CAPSULE_FLAGS_PERSIST_ACROSS_RESET 0x00010000
#define CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE 0x00020000
#define CAPSULE_FLAGS_INITIATE_RESET 0x00040000

typedef Status(EFIAPI *FnUpdateCapsule)(
		CapsuleHeader **CapsuleHeaderArray,
		uint64_t CapsuleCount,
		uintptr_t ScatterGatherList OPTIONAL);

typedef Status(EFIAPI *FnQueryCapsuleCapabilities)(
		CapsuleHeader **CapsuleHeaderArray,
		uint64_t CapsuleCount,
		uint64_t *MaximumCapsuleSize,
		ResetType *ResetType);

typedef Status(EFIAPI *FnQueryVariableInfo)(
		uint32_t Attributes,
		uint64_t *MaximumVariableStorageSize,
		uint64_t *RemainingVariableStorageSize,
		uint64_t *MaximumVariableSize);

//
// Protocol handler functions
//

typedef enum {
	EFI_NATIVE_INTERFACE,
	EFI_PCODE_INTERFACE
} InterfaceType;

typedef Status(EFIAPI *FnInstallProtocolInterface)(
		Handle *Handle,
		UUID *Protocol,
		InterfaceType InterfaceType,
		void *Interface);

typedef Status(EFIAPI *FnReinstallProtocolInterface)(
		Handle handle,
		UUID *Protocol,
		void *OldInterface,
		void *NewInterface);

typedef Status(EFIAPI *FnUninstallProtocolInterface)(
		Handle handle,
		UUID *Protocol,
		void *Interface);

typedef Status(EFIAPI *FnHandleProtocol)(
		Handle handle,
		UUID *Protocol,
		void **Interface);

typedef Status(EFIAPI *FnRegisterProtocolNotify)(
		UUID *Protocol,
		Event Event,
		void **Registration);

typedef Status(EFIAPI *FnLocateHandle)(
		LocateSearchType SearchType,
		UUID *Protocol OPTIONAL,
		void *SearchKey OPTIONAL,
		uint64_t *BufferSize,
		Handle *Buffer);

typedef Status(EFIAPI *FnLocateDevicePath)(
		UUID *Protocol,
		DevicePath **DevicePath,
		Handle *Device);

typedef Status(EFIAPI *FnInstallConfigurationTable)(
		UUID *Guid,
		void *Table);

typedef Status(EFIAPI *FnReservedService)();

//
// Standard  table header
//

typedef struct {
	uint64_t Signature;
	uint32_t Revision;
	uint32_t HeaderSize;
	uint32_t CRC32;
	uint32_t Reserved;
} TableHeader;

//
//  Runtime Serivces Table
//

#define EFI_RUNTIME_SERVICES_SIGNATURE 0x56524553544e5552
#define EFI_RUNTIME_SERVICES_REVISION (EFI_SPECIFICATION_MAJOR_REVISION << 16) | (EFI_SPECIFICATION_MINOR_REVISION)

typedef struct {
	TableHeader Hdr;

	//
	// Time services
	//

	FnGetTime GetTime;
	FnSetTime SetTime;
	FnGetWakeupTime GetWakeupTime;
	FnSetWakeupTime SetWakeupTime;

	//
	// Virtual memory services
	//

	FnSetVituralMemoryMap SetVirtualAddressMap;
	FnConvertRuntimePointer ConvertPointer;

	//
	// Variable serviers
	//

	FnGetVariable GetVariable;
	FnGetNextVariableName GetNextVariableName;
	FnSetVariable SetVariable;

	//
	// Misc
	//

	FnGetNextHighMonotonicCount GetNextHighMonotonicCount;
	FnResetSystem ResetSystem;

	FnUpdateCapsule UpdateCapsule;
	FnQueryCapsuleCapabilities QueryCapsuleCapabilities;
	FnQueryVariableInfo QueryVariableInfo;
} RuntimeServices;

//
//  Boot Services Table
//

#define EFI_BOOT_SERVICES_SIGNATURE 0x56524553544f4f42
#define EFI_BOOT_SERVICES_REVISION (EFI_SPECIFICATION_MAJOR_REVISION << 16) | (EFI_SPECIFICATION_MINOR_REVISION)

typedef struct __BOOT_SERVICES {
	TableHeader Hdr;

	//
	// Task priority functions
	//

	FnRaiseTpl RaiseTPL;
	FnRestoreTpl RestoreTPL;

	//
	// Memory functions
	//

	FnAllocatePages AllocatePages;
	FnFreePages FreePages;
	FnGetMemoryMap GetMemoryMap;
	FnAllocatePool AllocatePool;
	FnFreePool FreePool;

	//
	// Event & timer functions
	//

	FnCreateEvent CreateEvent;
	FnSetTimer SetTimer;
	FnWaitForEvent WaitForEvent;
	FnSignalEvent SignalEvent;
	FnCloseEvent CloseEvent;
	FnCheckEvent CheckEvent;

	//
	// Protocol handler functions
	//

	FnInstallProtocolInterface InstallProtocolInterface;
	FnReinstallProtocolInterface ReinstallProtocolInterface;
	FnUninstallProtocolInterface UninstallProtocolInterface;
	FnHandleProtocol HandleProtocol;
	FnHandleProtocol PCHandleProtocol;
	FnRegisterProtocolNotify RegisterProtocolNotify;
	FnLocateHandle LocateHandle;
	FnLocateDevicePath LocateDevicePath;
	FnInstallConfigurationTable InstallConfigurationTable;

	//
	// Image functions
	//

	FnLoadImage LoadImage;
	FnStartImage StartImage;
	FnExit Exit;
	FnImageUnload UnloadImage;
	FnExitBootServices ExitBootServices;

	//
	// Misc functions
	//

	FnGetNextMonotonicCount GetNextMonotonicCount;
	FnStall Stall;
	FnSetWatchdogTimer SetWatchdogTimer;

	//
	// DriverSupport Services
	//

	FnConnectController ConnectController;
	FnDisconnectController DisconnectController;

	//
	// Open and Close Protocol Services
	//
	FnOpenProtocol OpenProtocol;
	FnCloseProtocol CloseProtocol;
	FnOpenProtocolInfomation OpenProtocolInformation;

	//
	// Library Services
	//
	FnProtocolsPerHandles ProtocolsPerHandle;
	FnLocateHandleBuffer LocateHandleBuffer;
	FnLocateProtocol LocateProtocol;
	FnInstallMultipleProtocolInterfaces InstallMultipleProtocolInterfaces;
	FnUninstallMultipleProtocolInterfaces UninstallMultipleProtocolInterfaces;

	//
	// 32-bit CRC Services
	//
	FnCalculateCrc32 CalculateCrc32;

	//
	// Misc Services
	//
	FnMemcpy CopyMem;
	FnMemset SetMem;
	FnCreateEventEx CreateEventEx;
} BootServices;

//
//  Configuration Table and GUID dnitions
//

#define MPS_TABLE_GUID                              \
	{                                                 \
		0xeb9d2d2f, 0x2d88, 0x11d3, {                   \
			0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d \
		}                                               \
	}

#define ACPI_TABLE_GUID                             \
	{                                                 \
		0xeb9d2d30, 0x2d88, 0x11d3, {                   \
			0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d \
		}                                               \
	}

#define ACPI_20_TABLE_GUID                          \
	{                                                 \
		0x8868e871, 0xe4f1, 0x11d3, {                   \
			0xbc, 0x22, 0x0, 0x80, 0xc7, 0x3c, 0x88, 0x81 \
		}                                               \
	}

#define SMBIOS_TABLE_GUID                           \
	{                                                 \
		0xeb9d2d31, 0x2d88, 0x11d3, {                   \
			0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d \
		}                                               \
	}

#define SMBIOS3_TABLE_GUID                           \
	{                                                  \
		0xf2fd1544, 0x9794, 0x4a2c, {                    \
			0x99, 0x2e, 0xe5, 0xbb, 0xcf, 0x20, 0xe3, 0x94 \
		}                                                \
	}

#define SAL_SYSTEM_TABLE_GUID                       \
	{                                                 \
		0xeb9d2d32, 0x2d88, 0x11d3, {                   \
			0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d \
		}                                               \
	}

typedef struct __CONFIGURATION_TABLE {
	UUID guid;
	void *table;
} ConfigurationTable;

//
//  System Table
//

#define EFI_SYSTEM_TABLE_SIGNATURE 0x5453595320494249
#define EFI_SYSTEM_TABLE_REVISION (EFI_SPECIFICATION_MAJOR_REVISION << 16) | (EFI_SPECIFICATION_MINOR_REVISION)

typedef struct _SystemTable {
	TableHeader Hdr;

	const unsigned short *FirmwareVendor;
	uint32_t FirmwareRevision;

	Handle ConsoleInHandle;
	SIMPLE_INPUT_INTERFACE *ConIn;

	Handle ConsoleOutHandle;
	SIMPLE_TEXT_OUTPUT_INTERFACE *ConOut;

	Handle StandardErrorHandle;
	SIMPLE_TEXT_OUTPUT_INTERFACE *StdErr;

	RuntimeServices *runtimeServices;
	BootServices *bootServices;

	uint64_t tableEntryCount;
	ConfigurationTable *configurationTable;

} SystemTable;

#endif
