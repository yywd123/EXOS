#ifndef _EFI_PCI_IO_H
#define _EFI_PCI_IO_H

#define EFI_PCI_IO_PROTOCOL_GUID \
    { 0x4cf5b200, 0x68b8, 0x4ca5, {0x9e, 0xec, 0xb2, 0x3e, 0x3f, 0x50, 0x02, 0x9a} }

#define EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_GUID \
    { 0x2f707ebb, 0x4a1a, 0x11d4, {0x9a, 0x38, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d} }

INTERFACE_DECL(_EFI_PCI_IO_PROTOCOL);
INTERFACE_DECL(_EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL);

typedef enum {
    EfiPciIoWidthUint8,
    EfiPciIoWidthUint16,
    EfiPciIoWidthUint32,
    EfiPciIoWidthUint64,
    EfiPciIoWidthFifoUint8,
    EfiPciIoWidthFifoUint16,
    EfiPciIoWidthFifoUint32,
    EfiPciIoWidthFifoUint64,
    EfiPciIoWidthFillUint8,
    EfiPciIoWidthFillUint16,
    EfiPciIoWidthFillUint32,
    EfiPciIoWidthFillUint64,
    EfiPciIoWidthMaximum
} EFI_PCI_IO_PROTOCOL_WIDTH, EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH;

#define EFI_PCI_IO_PASS_THROUGH_BAR 0xff

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_IO_PROTOCOL_POLL_IO_MEM) (
  IN struct _EFI_PCI_IO_PROTOCOL  *This,
  IN EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN uint8_t                        BarIndex,
  IN uint64_t                       Offset,
  IN uint64_t                       Mask,
  IN uint64_t                       Value,
  IN uint64_t                       Delay,
  OUT uint64_t                      *Result
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_POLL_IO_MEM) (
  IN struct _EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    Width,
  IN uint64_t                                   Address,
  IN uint64_t                                   Mask,
  IN uint64_t                                   Value,
  IN uint64_t                                   Delay,
  OUT uint64_t                                  *Result
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_IO_PROTOCOL_IO_MEM) (
  IN struct _EFI_PCI_IO_PROTOCOL  *This,
  IN EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN uint8_t                        BarIndex,
  IN uint64_t                       Offset,
  IN uint64_t                        Count,
  IN OUT void                     *Buffer
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_IO_MEM) (
  IN struct _EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    Width,
  IN uint64_t                                   Address,
  IN uint64_t                                    Count,
  IN OUT void                                 *Buffer
);

typedef struct {
  EFI_PCI_IO_PROTOCOL_IO_MEM    Read;
  EFI_PCI_IO_PROTOCOL_IO_MEM    Write;
} EFI_PCI_IO_PROTOCOL_ACCESS;

typedef struct {
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_IO_MEM  Read;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_IO_MEM  Write;
} EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_ACCESS;

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_IO_PROTOCOL_CONFIG) (
  IN struct _EFI_PCI_IO_PROTOCOL  *This,
  IN EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN uint32_t                       Offset,
  IN uint64_t                        Count,
  IN OUT void                     *Buffer
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_CONFIGURATION) (
  IN struct _EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL    *This,
  OUT void                                      **Resources
);

typedef struct {
  EFI_PCI_IO_PROTOCOL_CONFIG Read;
  EFI_PCI_IO_PROTOCOL_CONFIG Write;
} EFI_PCI_IO_PROTOCOL_CONFIG_ACCESS;

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_IO_PROTOCOL_COPY_MEM) (
  IN struct _EFI_PCI_IO_PROTOCOL  *This,
  IN EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN uint8_t                        DestBarIndex,
  IN uint64_t                       DestOffset,
  IN uint8_t                        SrcBarIndex,
  IN uint64_t                       SrcOffset,
  IN uint64_t                        Count
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_COPY_MEM) (
  IN struct _EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    Width,
  IN uint64_t                                   DestAddress,
  IN uint64_t                                   SrcAddress,
  IN uint64_t                                    Count
);

typedef enum {
    EfiPciIoOperationBusMasterRead,
    EfiPciIoOperationBusMasterWrite,
    EfiPciIoOperationBusMasterCommonBuffer,
    EfiPciIoOperationMaximum
} EFI_PCI_IO_PROTOCOL_OPERATION;

typedef enum {
  EfiPciOperationBusMasterRead,
  EfiPciOperationBusMasterWrite,
  EfiPciOperationBusMasterCommonBuffer,
  EfiPciOperationBusMasterRead64,
  EfiPciOperationBusMasterWrite64,
  EfiPciOperationBusMasterCommonBuffer64,
  EfiPciOperationMaximum
} EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_OPERATION;

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_IO_PROTOCOL_MAP) (
  IN struct _EFI_PCI_IO_PROTOCOL   *This,
  IN EFI_PCI_IO_PROTOCOL_OPERATION Operation,
  IN void                          *HostAddress,
  IN OUT uint64_t                     *NumberOfBytes,
  OUT EFI_PHYSICAL_ADDRESS         *DeviceAddress,
  OUT void                         **Mapping
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_MAP) (
  IN struct _EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL    *This,
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_OPERATION  Operation,
  IN void                                       *HostAddress,
  IN OUT uint64_t                                  *NumberOfBytes,
  OUT EFI_PHYSICAL_ADDRESS                      *DeviceAddress,
  OUT void                                      **Mapping
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_IO_PROTOCOL_UNMAP) (
  IN struct _EFI_PCI_IO_PROTOCOL  *This,
  IN void                         *Mapping
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_UNMAP) (
  IN struct _EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL    *This,
  IN void                                       *Mapping
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_IO_PROTOCOL_ALLOCATE_BUFFER) (
  IN struct _EFI_PCI_IO_PROTOCOL  *This,
  IN EFI_ALLOCATE_TYPE            Type,
  IN EFI_MEMORY_TYPE              MemoryType,
  IN uint64_t                        Pages,
  OUT void                        **HostAddress,
  IN uint64_t                       Attributes
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_ALLOCATE_BUFFER) (
  IN struct _EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL    *This,
  IN EFI_ALLOCATE_TYPE                          Type,
  IN EFI_MEMORY_TYPE                            MemoryType,
  IN uint64_t                                      Pages,
  IN OUT void                                   **HostAddress,
  IN uint64_t                                     Attributes
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_IO_PROTOCOL_FREE_BUFFER) (
  IN struct _EFI_PCI_IO_PROTOCOL  *This,
  IN uint64_t                        Pages,
  IN void                         *HostAddress
  );

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_FREE_BUFFER) (
  IN struct _EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL    *This,
  IN uint64_t                                      Pages,
  IN void                                       *HostAddress
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_IO_PROTOCOL_FLUSH) (
  IN struct _EFI_PCI_IO_PROTOCOL  *This
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_FLUSH) (
  IN struct _EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL    *This
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_IO_PROTOCOL_GET_LOCATION) (
  IN struct _EFI_PCI_IO_PROTOCOL  *This,
  OUT uint64_t                       *SegmentNumber,
  OUT uint64_t                       *BusNumber,
  OUT uint64_t                       *DeviceNumber,
  OUT uint64_t                       *FunctionNumber
);

#define EFI_PCI_ATTRIBUTE_ISA_MOTHERBOARD_IO      0x0001
#define EFI_PCI_ATTRIBUTE_ISA_IO                  0x0002
#define EFI_PCI_ATTRIBUTE_VGA_PALETTE_IO          0x0004
#define EFI_PCI_ATTRIBUTE_VGA_MEMORY              0x0008
#define EFI_PCI_ATTRIBUTE_VGA_IO                  0x0010
#define EFI_PCI_ATTRIBUTE_IDE_PRIMARY_IO          0x0020
#define EFI_PCI_ATTRIBUTE_IDE_SECONDARY_IO        0x0040
#define EFI_PCI_ATTRIBUTE_MEMORY_WRITE_COMBINE    0x0080
#define EFI_PCI_ATTRIBUTE_IO                      0x0100
#define EFI_PCI_ATTRIBUTE_MEMORY                  0x0200
#define EFI_PCI_ATTRIBUTE_BUS_MASTER              0x0400
#define EFI_PCI_ATTRIBUTE_MEMORY_CACHED           0x0800
#define EFI_PCI_ATTRIBUTE_MEMORY_DISABLE          0x1000
#define EFI_PCI_ATTRIBUTE_EMBEDDED_DEVICE         0x2000
#define EFI_PCI_ATTRIBUTE_EMBEDDED_ROM            0x4000
#define EFI_PCI_ATTRIBUTE_DUAL_ADDRESS_CYCLE      0x8000
#define EFI_PCI_ATTRIBUTE_ISA_IO_16               0x10000
#define EFI_PCI_ATTRIBUTE_VGA_PALETTE_IO_16       0x20000
#define EFI_PCI_ATTRIBUTE_VGA_IO_16               0x40000

#define EFI_PCI_IO_ATTRIBUTE_ISA_MOTHERBOARD_IO   EFI_PCI_ATTRIBUTE_ISA_MOTHERBOARD_IO
#define EFI_PCI_IO_ATTRIBUTE_ISA_IO               EFI_PCI_ATTRIBUTE_ISA_IO
#define EFI_PCI_IO_ATTRIBUTE_VGA_PALETTE_IO       EFI_PCI_ATTRIBUTE_VGA_PALETTE_IO
#define EFI_PCI_IO_ATTRIBUTE_VGA_MEMORY           EFI_PCI_ATTRIBUTE_VGA_MEMORY
#define EFI_PCI_IO_ATTRIBUTE_VGA_IO               EFI_PCI_ATTRIBUTE_VGA_IO
#define EFI_PCI_IO_ATTRIBUTE_IDE_PRIMARY_IO       EFI_PCI_ATTRIBUTE_IDE_PRIMARY_IO
#define EFI_PCI_IO_ATTRIBUTE_IDE_SECONDARY_IO     EFI_PCI_ATTRIBUTE_IDE_SECONDARY_IO
#define EFI_PCI_IO_ATTRIBUTE_MEMORY_WRITE_COMBINE EFI_PCI_ATTRIBUTE_MEMORY_WRITE_COMBINE
#define EFI_PCI_IO_ATTRIBUTE_IO                   EFI_PCI_ATTRIBUTE_IO
#define EFI_PCI_IO_ATTRIBUTE_MEMORY               EFI_PCI_ATTRIBUTE_MEMORY
#define EFI_PCI_IO_ATTRIBUTE_BUS_MASTER           EFI_PCI_ATTRIBUTE_BUS_MASTER
#define EFI_PCI_IO_ATTRIBUTE_MEMORY_CACHED        EFI_PCI_ATTRIBUTE_MEMORY_CACHED
#define EFI_PCI_IO_ATTRIBUTE_MEMORY_DISABLE       EFI_PCI_ATTRIBUTE_MEMORY_DISABLE
#define EFI_PCI_IO_ATTRIBUTE_EMBEDDED_DEVICE      EFI_PCI_ATTRIBUTE_EMBEDDED_DEVICE
#define EFI_PCI_IO_ATTRIBUTE_EMBEDDED_ROM         EFI_PCI_ATTRIBUTE_EMBEDDED_ROM
#define EFI_PCI_IO_ATTRIBUTE_DUAL_ADDRESS_CYCLE   EFI_PCI_ATTRIBUTE_DUAL_ADDRESS_CYCLE
#define EFI_PCI_IO_ATTRIBUTE_ISA_IO_16            EFI_PCI_ATTRIBUTE_ISA_IO_16
#define EFI_PCI_IO_ATTRIBUTE_VGA_PALETTE_IO_16    EFI_PCI_ATTRIBUTE_VGA_PALETTE_IO_16
#define EFI_PCI_IO_ATTRIBUTE_VGA_IO_16            EFI_PCI_ATTRIBUTE_VGA_IO_16

#define EFI_PCI_ATTRIBUTE_VALID_FOR_ALLOCATE_BUFFER \
    (EFI_PCI_ATTRIBUTE_MEMORY_WRITE_COMBINE | EFI_PCI_ATTRIBUTE_MEMORY_CACHED | EFI_PCI_ATTRIBUTE_DUAL_ADDRESS_CYCLE)

#define EFI_PCI_ATTRIBUTE_INVALID_FOR_ALLOCATE_BUFFER \
    (~EFI_PCI_ATTRIBUTE_VALID_FOR_ALLOCATE_BUFFER)

typedef struct {
    uint8_t Register;
    uint8_t Function;
    uint8_t Device;
    uint8_t Bus;
    uint32_t ExtendedRegister;
} EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS;

typedef enum {
    EfiPciIoAttributeOperationGet,
    EfiPciIoAttributeOperationSet,
    EfiPciIoAttributeOperationEnable,
    EfiPciIoAttributeOperationDisable,
    EfiPciIoAttributeOperationSupported,
    EfiPciIoAttributeOperationMaximum
} EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION;

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_IO_PROTOCOL_ATTRIBUTES) (
  IN struct _EFI_PCI_IO_PROTOCOL             *This,
  IN EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION Operation,
  IN uint64_t                                  Attributes,
  OUT uint64_t                                 *Result OPTIONAL
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_IO_PROTOCOL_GET_BAR_ATTRIBUTES) (
  IN struct _EFI_PCI_IO_PROTOCOL  *This,
  IN uint8_t                        BarIndex,
  OUT uint64_t                      *Supports   OPTIONAL,
  OUT void                        **Resources OPTIONAL
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_GET_ATTRIBUTES) (
  IN struct _EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL    *This,
  OUT uint64_t                                    *Supports,
  OUT uint64_t                                    *Attributes
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_IO_PROTOCOL_SET_BAR_ATTRIBUTES) (
  IN struct _EFI_PCI_IO_PROTOCOL  *This,
  IN uint64_t                       Attributes,
  IN uint8_t                        BarIndex,
  IN OUT uint64_t                   *Offset,
  IN OUT uint64_t                   *Length
);

typedef
EFI_STATUS
(EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_SET_ATTRIBUTES) (
  IN struct _EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL    *This,
  IN uint64_t                                     Attributes,
  IN OUT uint64_t                                 *ResourceBase,
  IN OUT uint64_t                                 *ResourceLength
);

typedef struct _EFI_PCI_IO_PROTOCOL {
  EFI_PCI_IO_PROTOCOL_POLL_IO_MEM        PollMem;
  EFI_PCI_IO_PROTOCOL_POLL_IO_MEM        PollIo;
  EFI_PCI_IO_PROTOCOL_ACCESS             Mem;
  EFI_PCI_IO_PROTOCOL_ACCESS             Io;
  EFI_PCI_IO_PROTOCOL_CONFIG_ACCESS      Pci;
  EFI_PCI_IO_PROTOCOL_COPY_MEM           CopyMem;
  EFI_PCI_IO_PROTOCOL_MAP                Map;
  EFI_PCI_IO_PROTOCOL_UNMAP              Unmap;
  EFI_PCI_IO_PROTOCOL_ALLOCATE_BUFFER    AllocateBuffer;
  EFI_PCI_IO_PROTOCOL_FREE_BUFFER        FreeBuffer;
  EFI_PCI_IO_PROTOCOL_FLUSH              Flush;
  EFI_PCI_IO_PROTOCOL_GET_LOCATION       GetLocation;
  EFI_PCI_IO_PROTOCOL_ATTRIBUTES         Attributes;
  EFI_PCI_IO_PROTOCOL_GET_BAR_ATTRIBUTES GetBarAttributes;
  EFI_PCI_IO_PROTOCOL_SET_BAR_ATTRIBUTES SetBarAttributes;
  uint64_t                                 RomSize;
  void                                   *RomImage;
} EFI_PCI_IO_PROTOCOL;

// Note: Because it conflicted with the EDK2 struct name, the
// 'EFI_PCI_IO_PROTOCOL' GUID definition, from older versions
// of gnu-efi, is now obsoleted.
// Use 'EFI_PCI_IO_PROTOCOL_GUID' instead.

typedef struct _EFI_PCI_IO_PROTOCOL _EFI_PCI_IO;
typedef EFI_PCI_IO_PROTOCOL EFI_PCI_IO;

typedef struct _EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL {
  EFI_HANDLE                                      ParentHandle;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_POLL_IO_MEM     PollMem;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_POLL_IO_MEM     PollIo;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_ACCESS          Mem;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_ACCESS          Io;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_ACCESS          Pci;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_COPY_MEM        CopyMem;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_MAP             Map;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_UNMAP           Unmap;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_ALLOCATE_BUFFER AllocateBuffer;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_FREE_BUFFER     FreeBuffer;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_FLUSH           Flush;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_GET_ATTRIBUTES  GetAttributes;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_SET_ATTRIBUTES  SetAttributes;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_CONFIGURATION   Configuration;
  uint32_t                                          SegmentNumber;
} EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL;

#endif /* _EFI_PCI_IO_H */
