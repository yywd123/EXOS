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
// Declare forward referenced data structures
//

INTERFACE_DECL(_EFI_SYSTEM_TABLE);

//
// EFI Memory
//

typedef EFI_STATUS
(EFIAPI *FnEfiAllocatePages) (
    IN EFI_ALLOCATE_TYPE            Type,
    IN EFI_MEMORY_TYPE              MemoryType,
    IN uint64_t                        NoPages,
    OUT EFI_PHYSICAL_ADDRESS        *Memory
    );

typedef EFI_STATUS
(EFIAPI *FnEfiFreePages) (
    IN EFI_PHYSICAL_ADDRESS         Memory,
    IN uint64_t                        NoPages
    );

typedef EFI_STATUS
(EFIAPI *FnEfiGetMemoryMap) (
    IN OUT uint64_t                    *MemoryMapSize,
    IN OUT EFI_MEMORY_DESCRIPTOR    *MemoryMap,
    OUT uint64_t                       *MapKey,
    OUT uint64_t                       *DescriptorSize,
    OUT uint32_t                      *DescriptorVersion
    );

#define NextMemoryDescriptor(Ptr,Size)  ((EFI_MEMORY_DESCRIPTOR *) (((uint8_t *) Ptr) + Size))

typedef EFI_STATUS
(EFIAPI *FnEfiAllocatePool) (
    IN EFI_MEMORY_TYPE              PoolType,
    IN uint64_t                        Size,
    OUT void                        **Buffer
    );

typedef EFI_STATUS
(EFIAPI *FnEfiFreePool) (
    IN void                         *Buffer
    );

typedef 
EFI_STATUS
(EFIAPI *FnEfiSetVituralMemoryMap) (
    IN uint64_t                        MemoryMapSize,
    IN uint64_t                        DescriptorSize,
    IN uint32_t                       DescriptorVersion,
    IN EFI_MEMORY_DESCRIPTOR        *VirtualMap
    );


#define EFI_OPTIONAL_PTR            0x00000001
#define EFI_INTERNAL_FNC            0x00000002      // Pointer to internal runtime fnc
#define EFI_INTERNAL_PTR            0x00000004      // Pointer to internal runtime data


typedef 
EFI_STATUS
(EFIAPI *FnEfiConvertRuntimePointer) (
    IN uint64_t                        DebugDisposition,
    IN OUT void                     **Address
    );


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
(EFIAPI *FnEfiEventNotify) (
    IN EFI_EVENT                Event,
    IN void                     *Context
    );

typedef EFI_STATUS
(EFIAPI *FnEfiCreateEvent) (
    IN uint32_t                       Type,
    IN EFI_TPL                      NotifyTpl,
    IN FnEfiEventNotify             NotifyFunction,
    IN void                         *NotifyContext,
    OUT EFI_EVENT                   *Event
    );

typedef enum {
    TimerCancel,
    TimerPeriodic,
    TimerRelative,
    TimerTypeMax
} EFI_TIMER_DELAY;

typedef EFI_STATUS
(EFIAPI *FnEfiSetTimer) (
    IN EFI_EVENT                Event,
    IN EFI_TIMER_DELAY          Type,
    IN uint64_t                   TriggerTime
    );

typedef EFI_STATUS
(EFIAPI *FnEfiSignalEvent) (
    IN EFI_EVENT                Event
    );

typedef 
EFI_STATUS
(EFIAPI *FnEfiWaitForEvent) (
    IN uint64_t                    NumberOfEvents,
    IN EFI_EVENT                *Event,
    OUT uint64_t                   *Index
    );

typedef EFI_STATUS
(EFIAPI *FnEfiCloseEvent) (
    IN EFI_EVENT                Event
    );

typedef EFI_STATUS
(EFIAPI *FnEfiCheckEvent) (
    IN EFI_EVENT                Event
    );

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
typedef EFI_TPL
(EFIAPI *FnEfiRaiseTpl) (
    IN EFI_TPL      NewTpl
    );

typedef void
(EFIAPI *FnEfiRestoreTpl) (
    IN EFI_TPL      OldTpl
    );


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

typedef EFI_STATUS
(EFIAPI *FnEfiGetVariable) (
    IN const unsigned short                       *VariableName,
    IN EFI_GUID                     *VendorGuid,
    OUT uint32_t                      *Attributes OPTIONAL,
    IN OUT uint64_t                    *DataSize,
    OUT void                        *Data
    );

typedef EFI_STATUS
(EFIAPI *FnEfiGetNextVariableName) (
    IN OUT uint64_t                    *VariableNameSize,
    IN OUT const unsigned short                   *VariableName,
    IN OUT EFI_GUID                 *VendorGuid
    );


typedef EFI_STATUS
(EFIAPI *FnEfiSetVariable) (
    IN const unsigned short                       *VariableName,
    IN EFI_GUID                     *VendorGuid,
    IN uint32_t                       Attributes,
    IN uint64_t                        DataSize,
    IN void                         *Data
    );


//
// EFI Time
//

typedef struct {
        uint32_t                      Resolution;     // 1e-6 parts per million
        uint32_t                      Accuracy;       // hertz
        bool                     SetsToZero;     // Set clears sub-second time
} EFI_TIME_CAPABILITIES;


typedef EFI_STATUS
(EFIAPI *FnEfiGetTime) (
    OUT EFI_TIME                    *Time,
    OUT EFI_TIME_CAPABILITIES       *Capabilities OPTIONAL
    );

typedef EFI_STATUS
(EFIAPI *FnEfiSetTime) (
    IN EFI_TIME                     *Time
    );

typedef EFI_STATUS
(EFIAPI *FnEfiGetWakeupTime) (
    OUT bool                     *Enabled,
    OUT bool                     *Pending,
    OUT EFI_TIME                    *Time
    );

typedef EFI_STATUS
(EFIAPI *FnEfiSetWakeupTime) (
    IN bool                      Enable,
    IN EFI_TIME                     *Time OPTIONAL
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
EFI_STATUS
(EFIAPI *FnEfiImageEntryPoint) (
    IN EFI_HANDLE                   ImageHandle,
    IN struct _EFI_SYSTEM_TABLE     *SystemTable
    );

typedef 
EFI_STATUS
(EFIAPI *FnEfiLoadImage) (
    IN bool                      BootPolicy,
    IN EFI_HANDLE                   ParentImageHandle,
    IN EFI_DEVICE_PATH              *FilePath,
    IN void                         *SourceBuffer   OPTIONAL,
    IN uint64_t                        SourceSize,
    OUT EFI_HANDLE                  *ImageHandle
    );

typedef 
EFI_STATUS
(EFIAPI *FnEfiStartImage) (
    IN EFI_HANDLE                   ImageHandle,
    OUT uint64_t                       *ExitDataSize,
    OUT const unsigned short                      **ExitData  OPTIONAL
    );

typedef EFI_STATUS
(EFIAPI *FnEfiExit) (
    IN EFI_HANDLE                   ImageHandle,
    IN EFI_STATUS                   ExitStatus,
    IN uint64_t                        ExitDataSize,
    IN const unsigned short                       *ExitData OPTIONAL
    );

typedef EFI_STATUS
(EFIAPI *FnEfiExitBootServices) (
    IN EFI_HANDLE                   ImageHandle,
    IN uint64_t                        MapKey
    );

//
// Misc
//

typedef EFI_STATUS
(EFIAPI *FnEfiStall) (
    IN uint64_t                    Microseconds
    );

typedef EFI_STATUS
(EFIAPI *FnEfiSetWatchdogTimer) (
    IN uint64_t                    Timeout,
    IN uint64_t                   WatchdogCode,
    IN uint64_t                    DataSize,
    IN const unsigned short                   *WatchdogData OPTIONAL
    );

typedef EFI_STATUS
(EFIAPI *FnEfiConnectController) (
    IN EFI_HANDLE               ControllerHandle,
    IN EFI_HANDLE               *DriverImageHandle OPTIONAL,
    IN EFI_DEVICE_PATH          *RemainingDevicePath OPTIONAL,
    IN bool                  Recursive
    );

typedef EFI_STATUS
(EFIAPI *FnEfiDisconnectController) (
    IN EFI_HANDLE               ControllerHandle,
    IN EFI_HANDLE               DriverImageHandle OPTIONAL,
    IN EFI_HANDLE               ChildHandle OPTIONAL
    );

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL  0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL        0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL       0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER           0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE           0x00000020

typedef EFI_STATUS
(EFIAPI *FnEfiOpenProtocol) (
    IN EFI_HANDLE               Handle,
    IN EFI_GUID                 *Protocol,
    OUT void                    **Interface OPTIONAL,
    IN EFI_HANDLE               AgentHandle,
    IN EFI_HANDLE               ControllerHandle,
    IN uint32_t                   Attributes
    );

typedef EFI_STATUS
(EFIAPI *FnEfiCloseProtocol) (
    IN EFI_HANDLE               Handle,
    IN EFI_GUID                 *Protocol,
    IN EFI_HANDLE               AgentHandle,
    IN EFI_HANDLE               ControllerHandle
    );

typedef struct {
    EFI_HANDLE                  AgentHandle;
    EFI_HANDLE                  ControllerHandle;
    uint32_t                      Attributes;
    uint32_t                      OpenCount;
} EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;

typedef EFI_STATUS
(EFIAPI *FnEfiOpenProtocolInfomation) (
    IN EFI_HANDLE               Handle,
    IN EFI_GUID                 *Protocol,
    OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY **EntryBuffer,
    OUT uint64_t                   *EntryCount
    );

typedef EFI_STATUS
(EFIAPI *FnEfiProtocolsPerHandles) (
    IN EFI_HANDLE               Handle,
    OUT EFI_GUID                ***ProtocolBuffer,
    OUT uint64_t                   *ProtocolBufferCount
    );

typedef enum {
    AllHandles,
    ByRegisterNotify,
    ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

typedef EFI_STATUS
(EFIAPI *FnEfiLocateHandleBuffer) (
    IN EFI_LOCATE_SEARCH_TYPE   SearchType,
    IN EFI_GUID                 *Protocol OPTIONAL,
    IN void                     *SearchKey OPTIONAL,
    IN OUT uint64_t                *NoHandles,
    OUT EFI_HANDLE              **Buffer
    );

typedef EFI_STATUS
(EFIAPI *FnEfiLocateProtocol) (
    IN EFI_GUID                 *Protocol,
    IN void                     *Registration OPTIONAL,
    OUT void                    **Interface
    );

typedef EFI_STATUS
(EFIAPI *FnEfiInstallMultipleProtocolInterfaces) (
    IN OUT EFI_HANDLE           *Handle,
    ...
    );

typedef EFI_STATUS
(EFIAPI *FnEfiUninstallMultipleProtocolInterfaces) (
    IN OUT EFI_HANDLE           Handle,
    ...
    );

typedef EFI_STATUS
(EFIAPI *FnEfiCalculateCrc32) (
    IN void                     *Data,
    IN uint64_t                    DataSize,
    OUT uint32_t                  *Crc32
    );

typedef void
(EFIAPI *FnEfiMemcpy) (
    IN void                     *Destination,
    IN void                     *Source,
    IN uint64_t                    Length
    );

typedef void
(EFIAPI *FnEfiMemset) (
    IN void                     *Buffer,
    IN uint64_t                    Size,
    IN uint8_t                    Value
    );


typedef EFI_STATUS
(EFIAPI *FnEfiCreateEventEx) (
    IN uint32_t                   Type,
    IN EFI_TPL                  NotifyTpl,
    IN FnEfiEventNotify         NotifyFunction OPTIONAL,
    IN const void               *NotifyContext OPTIONAL,
    IN const EFI_GUID           EventGroup OPTIONAL,
    OUT EFI_EVENT               *Event
    );

typedef enum {
    EfiResetCold,
    EfiResetWarm,
    EfiResetShutdown
} EFI_RESET_TYPE;

typedef EFI_STATUS
(EFIAPI *FnEfiResetSystem) (
    IN EFI_RESET_TYPE           ResetType,
    IN EFI_STATUS               ResetStatus,
    IN uint64_t                    DataSize,
    IN const unsigned short                   *ResetData OPTIONAL
    );

typedef EFI_STATUS
(EFIAPI *FnEfiGetNextMonotonicCount) (
    OUT uint64_t                  *Count
    );

typedef EFI_STATUS
(EFIAPI *FnEfiGetNextHighMonotonicCount) (
    OUT uint32_t                  *HighCount
    );

typedef struct {
    uint64_t                      Length;
    union {
        EFI_PHYSICAL_ADDRESS    DataBlock;
       EFI_PHYSICAL_ADDRESS    ContinuationPointer;
    } Union;
} EFI_CAPSULE_BLOCK_DESCRIPTOR;

typedef struct {
    EFI_GUID                    CapsuleGuid;
    uint32_t                      HeaderSize;
    uint32_t                      Flags;
    uint32_t                      CapsuleImageSize;
} EFI_CAPSULE_HEADER;

#define CAPSULE_FLAGS_PERSIST_ACROSS_RESET    0x00010000
#define CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE   0x00020000
#define CAPSULE_FLAGS_INITIATE_RESET          0x00040000

typedef EFI_STATUS
(EFIAPI *FnEfiUpdateCapsule) (
    IN EFI_CAPSULE_HEADER       **CapsuleHeaderArray,
    IN uint64_t                    CapsuleCount,
    IN EFI_PHYSICAL_ADDRESS     ScatterGatherList OPTIONAL
    );

typedef EFI_STATUS
(EFIAPI *FnEfiQueryCapsuleCapabilities) (
    IN  EFI_CAPSULE_HEADER       **CapsuleHeaderArray,
    IN  uint64_t                    CapsuleCount,
    OUT uint64_t                   *MaximumCapsuleSize,
    OUT EFI_RESET_TYPE           *ResetType
    );

typedef EFI_STATUS
(EFIAPI *FnEfiQueryVariableInfo) (
    IN  uint32_t                  Attributes,
    OUT uint64_t                  *MaximumVariableStorageSize,
    OUT uint64_t                  *RemainingVariableStorageSize,
    OUT uint64_t                  *MaximumVariableSize
    );

//
// Protocol handler functions
//

typedef enum {
    EFI_NATIVE_INTERFACE,
    EFI_PCODE_INTERFACE
} EFI_INTERFACE_TYPE;

typedef EFI_STATUS
(EFIAPI *FnEfiInstallProtocolInterface) (
    IN OUT EFI_HANDLE           *Handle,
    IN EFI_GUID                 *Protocol,
    IN EFI_INTERFACE_TYPE       InterfaceType,
    IN void                     *Interface
    );

typedef EFI_STATUS
(EFIAPI *FnEfiReinstallProtocolInterface) (
    IN EFI_HANDLE               Handle,
    IN EFI_GUID                 *Protocol,
    IN void                     *OldInterface,
    IN void                     *NewInterface
    );

typedef EFI_STATUS
(EFIAPI *FnEfiUninstallProtocolInterface) (
    IN EFI_HANDLE               Handle,
    IN EFI_GUID                 *Protocol,
    IN void                     *Interface
    );

typedef EFI_STATUS
(EFIAPI *FnEfiHandleProtocol) (
    IN EFI_HANDLE               Handle,
    IN EFI_GUID                 *Protocol,
    OUT void                    **Interface
    );

typedef EFI_STATUS 
(EFIAPI *FnEfiRegisterProtocolNotify) (
    IN EFI_GUID                 *Protocol,
    IN EFI_EVENT                Event,
    OUT void                    **Registration
    );

typedef EFI_STATUS
(EFIAPI *FnEfiLocateHandle) (
    IN EFI_LOCATE_SEARCH_TYPE   SearchType,
    IN EFI_GUID                 *Protocol OPTIONAL,
    IN void                     *SearchKey OPTIONAL,
    IN OUT uint64_t                *BufferSize,
    OUT EFI_HANDLE              *Buffer
    );

typedef EFI_STATUS
(EFIAPI *FnEfiLocateDevicePath) (
    IN EFI_GUID                 *Protocol,
    IN OUT EFI_DEVICE_PATH      **DevicePath,
    OUT EFI_HANDLE              *Device
    );

typedef EFI_STATUS
(EFIAPI *FnEfiInstallConfigurationTable) (
    IN EFI_GUID                 *Guid,
    IN void                     *Table
    );

typedef EFI_STATUS
(EFIAPI *FnEfiReservedService) (
    );

//
// Standard EFI table header
//

typedef struct _EFI_TABLE_HEADER {
    uint64_t                      Signature;
    uint32_t                      Revision;
    uint32_t                      HeaderSize;
    uint32_t                      CRC32;
    uint32_t                      Reserved;
} EFI_TABLE_HEADER;


//
// EFI Runtime Serivces Table
//

#define EFI_RUNTIME_SERVICES_SIGNATURE  0x56524553544e5552
#define EFI_RUNTIME_SERVICES_REVISION   (EFI_SPECIFICATION_MAJOR_REVISION<<16) | (EFI_SPECIFICATION_MINOR_REVISION)

typedef struct  {
    EFI_TABLE_HEADER                Hdr;

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
} EFI_RUNTIME_SERVICES;


//
// EFI Boot Services Table
//

#define EFI_BOOT_SERVICES_SIGNATURE     0x56524553544f4f42
#define EFI_BOOT_SERVICES_REVISION      (EFI_SPECIFICATION_MAJOR_REVISION<<16) | (EFI_SPECIFICATION_MINOR_REVISION)

typedef struct _EFI_BOOT_SERVICES {

    EFI_TABLE_HEADER                Hdr;

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
    EFI_IMAGE_UNLOAD                UnloadImage;
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
} EFI_BOOT_SERVICES;


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
    EFI_GUID                VendorGuid;
    void                    *VendorTable;
} EFI_CONFIGURATION_TABLE;


//
// EFI System Table
//




#define EFI_SYSTEM_TABLE_SIGNATURE      0x5453595320494249
#define EFI_SYSTEM_TABLE_REVISION      (EFI_SPECIFICATION_MAJOR_REVISION<<16) | (EFI_SPECIFICATION_MINOR_REVISION)

typedef struct _EFI_SYSTEM_TABLE {
    EFI_TABLE_HEADER                Hdr;

    const unsigned short                          *FirmwareVendor;
    uint32_t                          FirmwareRevision;

    EFI_HANDLE                      ConsoleInHandle;
    SIMPLE_INPUT_INTERFACE          *ConIn;

    EFI_HANDLE                      ConsoleOutHandle;
    SIMPLE_TEXT_OUTPUT_INTERFACE    *ConOut;

    EFI_HANDLE                      StandardErrorHandle;
    SIMPLE_TEXT_OUTPUT_INTERFACE    *StdErr;

    EFI_RUNTIME_SERVICES            *RuntimeServices;
    EFI_BOOT_SERVICES               *BootServices;

    uint64_t                           NumberOfTableEntries;
    EFI_CONFIGURATION_TABLE         *ConfigurationTable;

} EFI_SYSTEM_TABLE;

#endif

