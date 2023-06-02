#ifndef _EFI_API_H
#define _EFI_API_H

/*++

Copyright (c) 1998  Intel Corporation

Module Name:

	efiapi.h

Abstract:

	Global EFI runtime & boot service interfaces




Revision History

--*/

//
// EFI Specification Revision
//

#define EFI_SPECIFICATION_MAJOR_REVISION 1
#define EFI_SPECIFICATION_MINOR_REVISION 02

//
// EFI Memory
//

typedef Status
(EFIAPI *FnEfiAllocatePages) (EFI_ALLOCATE_TYPE type, EFI_MEMORY_TYPE memoryType, uint64_t pageCount, uintptr_t *memory);

typedef Status
(EFIAPI *FnEfiFreePages) (uintptr_t memory, uint64_t pageCount);

typedef Status
(EFIAPI *FnEfiGetMemoryMap) (uint64_t *memoryMapSize, EfiMemoryDescriptor *memoryMap, uint64_t *mapKey, uint64_t *descriptorSize, uint32_t *descriptorVersion);

#define NextMemoryDescriptor(Ptr,Size)  ((EfiMemoryDescriptor *) (((uint8_t *) Ptr) + Size))

typedef Status
(EFIAPI *FnEfiAllocatePool) (EFI_MEMORY_TYPE poolType, uint64_t size, void **buffer);

typedef Status
(EFIAPI *FnEfiFreePool) (void *buffer);

typedef 
Status
(EFIAPI *FnEfiSetVituralMemoryMap) (uint64_t memoryMapSize, uint64_t descriptorSize, uint32_t descriptorVersion, EfiMemoryDescriptor *virtualMap);

#define EFI_OPTIONAL_PTR            0x00000001
#define EFI_INTERNAL_FNC            0x00000002      // Pointer to internal runtime fnc
#define EFI_INTERNAL_PTR            0x00000004      // Pointer to internal runtime data

typedef 
Status
(EFIAPI *FnEfiConvertRuntimePointer) (uint64_t debugDisposition, void **address);

//
// EFI Events
//

#define EVT_TIMER                           0x80000000
#define EVT_RUNTIME                         0x40000000
#define EVT_RUNTIME_CONTEXT                 0x20000000

#define EVT_NOTIFY_WAIT                     0x00000100
#define EVT_NOTIFY_SIGNAL                   0x00000200

#define EVT_SIGNAL_EXIT_BOOT_SERVICES       0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE   0x60000202

#define EVT_EFI_SIGNAL_MASK                 0x000000FF
#define EVT_EFI_SIGNAL_MAX                  4

#define EFI_EVENT_TIMER                         EVT_TIMER
#define EFI_EVENT_RUNTIME                       EVT_RUNTIME
#define EFI_EVENT_RUNTIME_CONTEXT               EVT_RUNTIME_CONTEXT
#define EFI_EVENT_NOTIFY_WAIT                   EVT_NOTIFY_WAIT
#define EFI_EVENT_NOTIFY_SIGNAL                 EVT_NOTIFY_SIGNAL
#define EFI_EVENT_SIGNAL_EXIT_BOOT_SERVICES     EVT_SIGNAL_EXIT_BOOT_SERVICES
#define EFI_EVENT_SIGNAL_VIRTUAL_ADDRESS_CHANGE EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE
#define EFI_EVENT_EFI_SIGNAL_MASK               EVT_EFI_SIGNAL_MASK
#define EFI_EVENT_EFI_SIGNAL_MAX                EVT_EFI_SIGNAL_MAX

typedef void
(EFIAPI *FnEfiEventNotify) (EfiEvent event, void *context);

typedef Status
(EFIAPI *FnEfiCreateEvent) (uint32_t type, EfiTpl notifyTpl, FnEfiEventNotify notifyFunction, void *notifyContext, EfiEvent *event);

typedef enum {
	TimerCancel,
	TimerPeriodic,
	TimerRelative,
	TimerTypeMax
} EFI_TIMER_DELAY;

typedef Status
(EFIAPI *FnEfiSetTimer) (EfiEvent Event, EFI_TIMER_DELAY Type, uint64_t triggerTime);

typedef Status
(EFIAPI *FnEfiSignalEvent) (EfiEvent event);

typedef 
Status
(EFIAPI *FnEfiWaitForEvent) (uint64_t numberOfEvents, EfiEvent *event, uint64_t *index);

typedef Status
(EFIAPI *FnEfiCloseEvent) (EfiEvent event);

typedef Status
(EFIAPI *FnEfiCheckEvent) (EfiEvent event);

//
// Task priority level
//

#define TPL_APPLICATION       4
#define TPL_CALLBACK          8
#define TPL_NOTIFY           16
#define TPL_HIGH_LEVEL       31
#define EFI_TPL_APPLICATION  TPL_APPLICATION
#define EFI_TPL_CALLBACK     TPL_CALLBACK
#define EFI_TPL_NOTIFY       TPL_NOTIFY
#define EFI_TPL_HIGH_LEVEL   TPL_HIGH_LEVEL
typedef EfiTpl
(EFIAPI *FnEfiRaiseTpl) (EfiTpl newTpl);

typedef void
(EFIAPI *FnEfiRestoreTpl) (EfiTpl oldTpl);

//
// EFI platform varibles
//

#define EFI_GLOBAL_VARIABLE     \
	{ 0x8BE4DF61, 0x93CA, 0x11d2, {0xAA, 0x0D, 0x00, 0xE0, 0x98, 0x03, 0x2B, 0x8C} }

// Variable attributes
#define EFI_VARIABLE_NON_VOLATILE                          0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS                    0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS                        0x00000004
#define EFI_VARIABLE_HARDWARE_ERROR_RECORD                 0x00000008
#define EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS            0x00000010
#define EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS 0x00000020
#define EFI_VARIABLE_APPEND_WRITE                          0x00000040

// Variable size limitation
#define EFI_MAXIMUM_VARIABLE_SIZE           1024

typedef Status
(EFIAPI *FnEfiGetVariable) (
	const unsigned short                       *VariableName,
	Guid                     *VendorGuid,
	uint32_t                      *Attributes OPTIONAL,
	uint64_t                    *DataSize,
	void                        *Data
	);

typedef Status
(EFIAPI *FnEfiGetNextVariableName) (
	uint64_t                    *VariableNameSize,
	const unsigned short                   *VariableName,
	Guid                 *VendorGuid
	);


typedef Status
(EFIAPI *FnEfiSetVariable) (
	const unsigned short                       *VariableName,
	Guid                     *VendorGuid,
	uint32_t                       Attributes,
	uint64_t                        DataSize,
	void                         *Data
	);


//
// EFI Time
//

typedef struct {
		uint32_t                      Resolution;     // 1e-6 parts per million
		uint32_t                      Accuracy;       // hertz
		bool                     SetsToZero;     // Set clears sub-second time
} EFI_TIME_CAPABILITIES;


typedef Status
(EFIAPI *FnEfiGetTime) (
	EfiTime                    *Time,
	EFI_TIME_CAPABILITIES       *Capabilities OPTIONAL
	);

typedef Status
(EFIAPI *FnEfiSetTime) (
	EfiTime                     *Time
	);

typedef Status
(EFIAPI *FnEfiGetWakeupTime) (
	bool                     *Enabled,
	bool                     *Pending,
	EfiTime                    *Time
	);

typedef Status
(EFIAPI *FnEfiSetWakeupTime) (
	bool                      Enable,
	EfiTime                     *Time OPTIONAL
	);


//
// Image functions
//


// PE32+ Subsystem type for EFI images

#if !defined(IMAGE_SUBSYSTEM_EFI_APPLICATION)
#define IMAGE_SUBSYSTEM_EFI_APPLICATION             10
#define IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER     11
#define IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER          12
#endif

// PE32+ Machine type for EFI images

#if !defined(EFI_IMAGE_MACHINE_IA32)
#define EFI_IMAGE_MACHINE_IA32      0x014c
#endif

#if !defined(EFI_IMAGE_MACHINE_IA64)
#define EFI_IMAGE_MACHINE_IA64      0x0200
#endif

#if !defined(EFI_IMAGE_MACHINE_EBC)
#define EFI_IMAGE_MACHINE_EBC      0x0EBC
#endif

#if !defined(EFI_IMAGE_MACHINE_X64)
#define EFI_IMAGE_MACHINE_X64       0x8664
#endif

#if !defined(EFI_IMAGE_MACHINE_ARMTHUMB_MIXED)
#define EFI_IMAGE_MACHINE_ARMTHUMB_MIXED 0x01C2
#endif

#if !defined(EFI_IMAGE_MACHINE_AARCH64)
#define EFI_IMAGE_MACHINE_AARCH64   0xAA64
#endif

// Image Entry prototype

typedef 
Status
(EFIAPI *FnEfiImageEntryPoint) (
	Handle                   ImageHandle,
	struct _EfiSystemTable     *SystemTable
	);

typedef 
Status
(EFIAPI *FnEfiLoadImage) (
	bool                      BootPolicy,
	Handle                   ParentImageHandle,
	EfiDevicePath              *FilePath,
	void                         *SourceBuffer   OPTIONAL,
	uint64_t                        SourceSize,
	Handle                  *ImageHandle
	);

typedef 
Status
(EFIAPI *FnEfiStartImage) (
	Handle                   ImageHandle,
	uint64_t                       *ExitDataSize,
	const unsigned short                      **ExitData  OPTIONAL
	);

typedef Status
(EFIAPI *FnEfiExit) (
	Handle                   ImageHandle,
	Status                   ExitStatus,
	uint64_t                        ExitDataSize,
	const unsigned short                       *ExitData OPTIONAL
	);

typedef Status
(EFIAPI *FnEfiExitBootServices) (
	Handle                   ImageHandle,
	uint64_t                        MapKey
	);

//
// Misc
//

typedef Status
(EFIAPI *FnEfiStall) (
	uint64_t                    Microseconds
	);

typedef Status
(EFIAPI *FnEfiSetWatchdogTimer) (
	uint64_t                    Timeout,
	uint64_t                   WatchdogCode,
	uint64_t                    DataSize,
	const unsigned short                   *WatchdogData OPTIONAL
	);

typedef Status
(EFIAPI *FnEfiConnectController) (
	Handle               ControllerHandle,
	Handle               *DriverImageHandle OPTIONAL,
	EfiDevicePath          *RemainingDevicePath OPTIONAL,
	bool                  Recursive
	);

typedef Status
(EFIAPI *FnEfiDisconnectController) (
	Handle               ControllerHandle,
	Handle               DriverImageHandle OPTIONAL,
	Handle               ChildHandle OPTIONAL
	);

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL  0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL        0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL       0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER           0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE           0x00000020

typedef Status
(EFIAPI *FnEfiOpenProtocol) (
	Handle               handle,
	Guid                 *Protocol,
	void                    **Interface OPTIONAL,
	Handle               AgentHandle,
	Handle               ControllerHandle,
	uint32_t                   Attributes
	);

typedef Status
(EFIAPI *FnEfiCloseProtocol) (
	Handle               handle,
	Guid                 *Protocol,
	Handle               AgentHandle,
	Handle               ControllerHandle
	);

typedef struct {
	Handle                  AgentHandle;
	Handle                  ControllerHandle;
	uint32_t                      Attributes;
	uint32_t                      OpenCount;
} EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;

typedef Status
(EFIAPI *FnEfiOpenProtocolInfomation) (
	Handle               handle,
	Guid                 *Protocol,
	EFI_OPEN_PROTOCOL_INFORMATION_ENTRY **EntryBuffer,
	uint64_t                   *EntryCount
	);

typedef Status
(EFIAPI *FnEfiProtocolsPerHandles) (
	Handle               handle,
	Guid                ***ProtocolBuffer,
	uint64_t                   *ProtocolBufferCount
	);

typedef enum {
	AllHandles,
	ByRegisterNotify,
	ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

typedef Status
(EFIAPI *FnEfiLocateHandleBuffer) (
	EFI_LOCATE_SEARCH_TYPE   SearchType,
	Guid                 *Protocol OPTIONAL,
	void                     *SearchKey OPTIONAL,
	uint64_t                *NoHandles,
	Handle              **Buffer
	);

typedef Status
(EFIAPI *FnEfiLocateProtocol) (
	Guid                 *Protocol,
	void                     *Registration OPTIONAL,
	void                    **Interface
	);

typedef Status
(EFIAPI *FnEfiInstallMultipleProtocolInterfaces) (
	Handle           *Handle,
	...
	);

typedef Status
(EFIAPI *FnEfiUninstallMultipleProtocolInterfaces) (
	Handle           Handle,
	...
	);

typedef Status
(EFIAPI *FnEfiCalculateCrc32) (
	void                     *Data,
	uint64_t                    DataSize,
	uint32_t                  *Crc32
	);

typedef void
(EFIAPI *FnEfiMemcpy) (
	void                     *Destination,
	void                     *Source,
	uint64_t                    Length
	);

typedef void
(EFIAPI *FnEfiMemset) (
	void                     *Buffer,
	uint64_t                    Size,
	uint8_t                    Value
	);


typedef Status
(EFIAPI *FnEfiCreateEventEx) (
	uint32_t                   Type,
	EfiTpl                  NotifyTpl,
	FnEfiEventNotify         NotifyFunction OPTIONAL,
	const void               *NotifyContext OPTIONAL,
	const Guid           EventGroup OPTIONAL,
	EfiEvent               *Event
	);

typedef enum {
	EfiResetCold,
	EfiResetWarm,
	EfiResetShutdown
} EFI_RESET_TYPE;

typedef Status
(EFIAPI *FnEfiResetSystem) (
	EFI_RESET_TYPE           ResetType,
	Status               ResetStatus,
	uint64_t                    DataSize,
	const unsigned short                   *ResetData OPTIONAL
	);

typedef Status
(EFIAPI *FnEfiGetNextMonotonicCount) (
	uint64_t                  *Count
	);

typedef Status
(EFIAPI *FnEfiGetNextHighMonotonicCount) (
	uint32_t                  *HighCount
	);

typedef struct {
	uint64_t                      Length;
	union {
		uintptr_t    DataBlock;
	   uintptr_t    ContinuationPointer;
	} Union;
} EFI_CAPSULE_BLOCK_DESCRIPTOR;

typedef struct {
	Guid                    CapsuleGuid;
	uint32_t                      HeaderSize;
	uint32_t                      Flags;
	uint32_t                      CapsuleImageSize;
} EFI_CAPSULE_HEADER;

#define CAPSULE_FLAGS_PERSIST_ACROSS_RESET    0x00010000
#define CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE   0x00020000
#define CAPSULE_FLAGS_INITIATE_RESET          0x00040000

typedef Status
(EFIAPI *FnEfiUpdateCapsule) (
	EFI_CAPSULE_HEADER       **CapsuleHeaderArray,
	uint64_t                    CapsuleCount,
	uintptr_t     ScatterGatherList OPTIONAL
	);

typedef Status
(EFIAPI *FnEfiQueryCapsuleCapabilities) (
	 EFI_CAPSULE_HEADER       **CapsuleHeaderArray,
	 uint64_t                    CapsuleCount,
	uint64_t                   *MaximumCapsuleSize,
	EFI_RESET_TYPE           *ResetType
	);

typedef Status
(EFIAPI *FnEfiQueryVariableInfo) (
	 uint32_t                  Attributes,
	uint64_t                  *MaximumVariableStorageSize,
	uint64_t                  *RemainingVariableStorageSize,
	uint64_t                  *MaximumVariableSize
	);

//
// Protocol handler functions
//

typedef enum {
	EFI_NATIVE_INTERFACE,
	EFI_PCODE_INTERFACE
} EFI_INTERFACE_TYPE;

typedef Status
(EFIAPI *FnEfiInstallProtocolInterface) (
	Handle           *Handle,
	Guid                 *Protocol,
	EFI_INTERFACE_TYPE       InterfaceType,
	void                     *Interface
	);

typedef Status
(EFIAPI *FnEfiReinstallProtocolInterface) (
	Handle               handle,
	Guid                 *Protocol,
	void                     *OldInterface,
	void                     *NewInterface
	);

typedef Status
(EFIAPI *FnEfiUninstallProtocolInterface) (
	Handle               handle,
	Guid                 *Protocol,
	void                     *Interface
	);

typedef Status
(EFIAPI *FnEfiHandleProtocol) (
	Handle               handle,
	Guid                 *Protocol,
	void                    **Interface
	);

typedef Status 
(EFIAPI *FnEfiRegisterProtocolNotify) (
	Guid                 *Protocol,
	EfiEvent                Event,
	void                    **Registration
	);

typedef Status
(EFIAPI *FnEfiLocateHandle) (
	EFI_LOCATE_SEARCH_TYPE   SearchType,
	Guid                 *Protocol OPTIONAL,
	void                     *SearchKey OPTIONAL,
	uint64_t                *BufferSize,
	Handle              *Buffer
	);

typedef Status
(EFIAPI *FnEfiLocateDevicePath) (
	Guid                 *Protocol,
	EfiDevicePath      **DevicePath,
	Handle              *Device
	);

typedef Status
(EFIAPI *FnEfiInstallConfigurationTable) (
	Guid                 *Guid,
	void                     *Table
	);

typedef Status
(EFIAPI *FnEfiReservedService) (
	);

//
// Standard EFI table header
//

typedef struct {
	uint64_t                      Signature;
	uint32_t                      Revision;
	uint32_t                      HeaderSize;
	uint32_t                      CRC32;
	uint32_t                      Reserved;
} EfiTableHeader;


//
// EFI Runtime Serivces Table
//

#define EFI_RUNTIME_SERVICES_SIGNATURE  0x56524553544e5552
#define EFI_RUNTIME_SERVICES_REVISION   (EFI_SPECIFICATION_MAJOR_REVISION<<16) | (EFI_SPECIFICATION_MINOR_REVISION)

typedef struct  {
	EfiTableHeader                Hdr;

	//
	// Time services
	//

	FnEfiGetTime                    GetTime;
	FnEfiSetTime                    SetTime;
	FnEfiGetWakeupTime             GetWakeupTime;
	FnEfiSetWakeupTime             SetWakeupTime;

	//
	// Virtual memory services
	//

	FnEfiSetVituralMemoryMap     SetVirtualAddressMap;
	FnEfiConvertRuntimePointer             ConvertPointer;

	//
	// Variable serviers
	//

	FnEfiGetVariable                GetVariable;
	FnEfiGetNextVariableName      GetNextVariableName;
	FnEfiSetVariable                SetVariable;

	//
	// Misc
	//

	FnEfiGetNextHighMonotonicCount    GetNextHighMonotonicCount;
	FnEfiResetSystem                ResetSystem;

	FnEfiUpdateCapsule              UpdateCapsule;
	FnEfiQueryCapsuleCapabilities  QueryCapsuleCapabilities;
	FnEfiQueryVariableInfo         QueryVariableInfo;
} EfiRuntimeServices;


//
// EFI Boot Services Table
//

#define EFI_BOOT_SERVICES_SIGNATURE     0x56524553544f4f42
#define EFI_BOOT_SERVICES_REVISION      (EFI_SPECIFICATION_MAJOR_REVISION<<16) | (EFI_SPECIFICATION_MINOR_REVISION)

typedef struct _EFI_BOOT_SERVICES {

	EfiTableHeader                Hdr;

	//
	// Task priority functions
	//

	FnEfiRaiseTpl                   RaiseTPL;
	FnEfiRestoreTpl                 RestoreTPL;

	//
	// Memory functions
	//

	FnEfiAllocatePages              AllocatePages;
	FnEfiFreePages                  FreePages;
	FnEfiGetMemoryMap              GetMemoryMap;
	FnEfiAllocatePool               AllocatePool;
	FnEfiFreePool                   FreePool;

	//
	// Event & timer functions
	//

	FnEfiCreateEvent                CreateEvent;
	FnEfiSetTimer                   SetTimer;
	FnEfiWaitForEvent              WaitForEvent;
	FnEfiSignalEvent                SignalEvent;
	FnEfiCloseEvent                 CloseEvent;
	FnEfiCheckEvent                 CheckEvent;

	//
	// Protocol handler functions
	//

	FnEfiInstallProtocolInterface  InstallProtocolInterface;
	FnEfiReinstallProtocolInterface ReinstallProtocolInterface;
	FnEfiUninstallProtocolInterface UninstallProtocolInterface;
	FnEfiHandleProtocol             HandleProtocol;
	FnEfiHandleProtocol             PCHandleProtocol;
	FnEfiRegisterProtocolNotify    RegisterProtocolNotify;
	FnEfiLocateHandle               LocateHandle;
	FnEfiLocateDevicePath          LocateDevicePath;
	FnEfiInstallConfigurationTable InstallConfigurationTable;

	//
	// Image functions
	//

	FnEfiLoadImage                  LoadImage;
	FnEfiStartImage                 StartImage;
	FnEfiExit                        Exit;
	FnEfiImageUnload                UnloadImage;
	FnEfiExitBootServices          ExitBootServices;

	//
	// Misc functions
	//

	FnEfiGetNextMonotonicCount    GetNextMonotonicCount;
	FnEfiStall                       Stall;
	FnEfiSetWatchdogTimer          SetWatchdogTimer;

	//
	// DriverSupport Services
	//

	FnEfiConnectController          ConnectController;
	FnEfiDisconnectController       DisconnectController;

	//
	// Open and Close Protocol Services
	//
	FnEfiOpenProtocol               OpenProtocol;
	FnEfiCloseProtocol              CloseProtocol;
	FnEfiOpenProtocolInfomation   OpenProtocolInformation;

	//
	// Library Services
	//
	FnEfiProtocolsPerHandles        ProtocolsPerHandle;
	FnEfiLocateHandleBuffer        LocateHandleBuffer;
	FnEfiLocateProtocol             LocateProtocol;
	FnEfiInstallMultipleProtocolInterfaces InstallMultipleProtocolInterfaces;
	FnEfiUninstallMultipleProtocolInterfaces UninstallMultipleProtocolInterfaces;

	//
	// 32-bit CRC Services
	//
	FnEfiCalculateCrc32             CalculateCrc32;

	//
	// Misc Services
	//
	FnEfiMemcpy                    CopyMem;
	FnEfiMemset                     SetMem;
	FnEfiCreateEventEx             CreateEventEx;
} EfiBootServices;


//
// EFI Configuration Table and GUID definitions
//

#define MPS_TABLE_GUID    \
	{ 0xeb9d2d2f, 0x2d88, 0x11d3, {0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d} }

#define ACPI_TABLE_GUID    \
	{ 0xeb9d2d30, 0x2d88, 0x11d3, {0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d} }

#define ACPI_20_TABLE_GUID  \
	{ 0x8868e871, 0xe4f1, 0x11d3, {0xbc, 0x22, 0x0, 0x80, 0xc7, 0x3c, 0x88, 0x81} }

#define SMBIOS_TABLE_GUID    \
	{ 0xeb9d2d31, 0x2d88, 0x11d3, {0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d} }

#define SMBIOS3_TABLE_GUID    \
	{ 0xf2fd1544, 0x9794, 0x4a2c, {0x99, 0x2e, 0xe5, 0xbb, 0xcf, 0x20, 0xe3, 0x94} }

#define SAL_SYSTEM_TABLE_GUID    \
	{ 0xeb9d2d32, 0x2d88, 0x11d3, {0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d} }


typedef struct _EFI_CONFIGURATION_TABLE {
	Guid                VendorGuid;
	void                    *VendorTable;
} EFI_CONFIGURATION_TABLE;


//
// EFI System Table
//




#define EFI_SYSTEM_TABLE_SIGNATURE      0x5453595320494249
#define EFI_SYSTEM_TABLE_REVISION      (EFI_SPECIFICATION_MAJOR_REVISION<<16) | (EFI_SPECIFICATION_MINOR_REVISION)

typedef struct _EfiSystemTable {
	EfiTableHeader                Hdr;

	const unsigned short                          *FirmwareVendor;
	uint32_t                          FirmwareRevision;

	Handle                      ConsoleInHandle;
	SIMPLE_INPUT_INTERFACE          *ConIn;

	Handle                      ConsoleOutHandle;
	SIMPLE_TEXT_OUTPUT_INTERFACE    *ConOut;

	Handle                      StandardErrorHandle;
	SIMPLE_TEXT_OUTPUT_INTERFACE    *StdErr;

	EfiRuntimeServices            *RuntimeServices;
	EfiBootServices               *BootServices;

	uint64_t                           NumberOfTableEntries;
	EFI_CONFIGURATION_TABLE         *ConfigurationTable;

} EfiSystemTable;

#endif

