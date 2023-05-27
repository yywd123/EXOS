#ifndef _EFINET_H
#define _EFINET_H


/*++
Copyright (c) 1999  Intel Corporation

Module Name:
    efinet.h

Abstract:
    EFI Simple Network protocol

Revision History
--*/


///////////////////////////////////////////////////////////////////////////////
//
//      Simple Network Protocol
//

#define EFI_SIMPLE_NETWORK_PROTOCOL_GUID \
    { 0xA19832B9, 0xAC25, 0x11D3, {0x9A, 0x2D, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D} }

INTERFACE_DECL(_EFI_SIMPLE_NETWORK_PROTOCOL);

///////////////////////////////////////////////////////////////////////////////
//

typedef struct {
    //
    // Total number of frames received.  Includes frames with errors and
    // dropped frames.
    //
    uint64_t  RxTotalFrames;

    //
    // Number of valid frames received and copied into receive buffers.
    //
    uint64_t  RxGoodFrames;

    //
    // Number of frames below the minimum length for the media.
    // This would be <64 for ethernet.
    //
    uint64_t  RxUndersizeFrames;

    //
    // Number of frames longer than the maxminum length for the
    // media.  This would be >1500 for ethernet.
    //
    uint64_t  RxOversizeFrames;

    //
    // Valid frames that were dropped because receive buffers were full.
    //
    uint64_t  RxDroppedFrames;

    //
    // Number of valid unicast frames received and not dropped.
    //
    uint64_t  RxUnicastFrames;

    //
    // Number of valid broadcast frames received and not dropped.
    //
    uint64_t  RxBroadcastFrames;

    //
    // Number of valid mutlicast frames received and not dropped.
    //
    uint64_t  RxMulticastFrames;

    //
    // Number of frames w/ CRC or alignment errors.
    //
    uint64_t  RxCrcErrorFrames;

    //
    // Total number of bytes received.  Includes frames with errors
    // and dropped frames.
    //
    uint64_t  RxTotalBytes;

    //
    // Transmit statistics.
    //
    uint64_t  TxTotalFrames;
    uint64_t  TxGoodFrames;
    uint64_t  TxUndersizeFrames;
    uint64_t  TxOversizeFrames;
    uint64_t  TxDroppedFrames;
    uint64_t  TxUnicastFrames;
    uint64_t  TxBroadcastFrames;
    uint64_t  TxMulticastFrames;
    uint64_t  TxCrcErrorFrames;
    uint64_t  TxTotalBytes;

    //
    // Number of collisions detection on this subnet.
    //
    uint64_t  Collisions;

    //
    // Number of frames destined for unsupported protocol.
    //
    uint64_t  UnsupportedProtocol;

} EFI_NETWORK_STATISTICS;

///////////////////////////////////////////////////////////////////////////////
//

typedef enum {
    EfiSimpleNetworkStopped,
    EfiSimpleNetworkStarted,
    EfiSimpleNetworkInitialized,
    EfiSimpleNetworkMaxState
} EFI_SIMPLE_NETWORK_STATE;

///////////////////////////////////////////////////////////////////////////////
//

#define EFI_SIMPLE_NETWORK_RECEIVE_UNICAST               0x01
#define EFI_SIMPLE_NETWORK_RECEIVE_MULTICAST             0x02
#define EFI_SIMPLE_NETWORK_RECEIVE_BROADCAST             0x04
#define EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS           0x08
#define EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS_MULTICAST 0x10

///////////////////////////////////////////////////////////////////////////////
//

#define EFI_SIMPLE_NETWORK_RECEIVE_INTERRUPT        0x01
#define EFI_SIMPLE_NETWORK_TRANSMIT_INTERRUPT       0x02
#define EFI_SIMPLE_NETWORK_COMMAND_INTERRUPT        0x04
#define EFI_SIMPLE_NETWORK_SOFTWARE_INTERRUPT       0x08

///////////////////////////////////////////////////////////////////////////////
//
#define MAX_MCAST_FILTER_CNT    16
typedef struct {
    uint32_t                      State;
    uint32_t                      HwAddressSize;
    uint32_t                      MediaHeaderSize;
    uint32_t                      MaxPacketSize;
    uint32_t                      NvRamSize;
    uint32_t                      NvRamAccessSize;
    uint32_t                      ReceiveFilterMask;
    uint32_t                      ReceiveFilterSetting;
    uint32_t                      MaxMCastFilterCount;
    uint32_t                      MCastFilterCount;
    EFI_MAC_ADDRESS             MCastFilter[MAX_MCAST_FILTER_CNT];
    EFI_MAC_ADDRESS             CurrentAddress;
    EFI_MAC_ADDRESS             BroadcastAddress;
    EFI_MAC_ADDRESS             PermanentAddress;
    uint8_t                       IfType;
    bool                     MacAddressChangeable;
    bool                     MultipleTxSupported;
    bool                     MediaPresentSupported;
    bool                     MediaPresent;
} EFI_SIMPLE_NETWORK_MODE;

///////////////////////////////////////////////////////////////////////////////
//

typedef
EFI_STATUS
(EFIAPI *EFI_SIMPLE_NETWORK_START) (
    IN struct _EFI_SIMPLE_NETWORK_PROTOCOL  *This
);

///////////////////////////////////////////////////////////////////////////////
//

typedef
EFI_STATUS
(EFIAPI *EFI_SIMPLE_NETWORK_STOP) (
    IN struct _EFI_SIMPLE_NETWORK_PROTOCOL  *This
);

///////////////////////////////////////////////////////////////////////////////
//

typedef
EFI_STATUS
(EFIAPI *EFI_SIMPLE_NETWORK_INITIALIZE) (
    IN struct _EFI_SIMPLE_NETWORK_PROTOCOL  *This,
    IN uint64_t                                ExtraRxBufferSize  OPTIONAL,
    IN uint64_t                                ExtraTxBufferSize  OPTIONAL
);

///////////////////////////////////////////////////////////////////////////////
//

typedef
EFI_STATUS
(EFIAPI *EFI_SIMPLE_NETWORK_RESET) (
    IN struct _EFI_SIMPLE_NETWORK_PROTOCOL  *This,
    IN bool                              ExtendedVerification
);

///////////////////////////////////////////////////////////////////////////////
//

typedef
EFI_STATUS
(EFIAPI *EFI_SIMPLE_NETWORK_SHUTDOWN) (
    IN struct _EFI_SIMPLE_NETWORK_PROTOCOL  *This
);

///////////////////////////////////////////////////////////////////////////////
//

typedef
EFI_STATUS
(EFIAPI *EFI_SIMPLE_NETWORK_RECEIVE_FILTERS) (
    IN struct _EFI_SIMPLE_NETWORK_PROTOCOL  *This,
    IN uint32_t                               Enable,
    IN uint32_t                               Disable,
    IN bool                              ResetMCastFilter,
    IN uint64_t                                MCastFilterCnt     OPTIONAL,
    IN EFI_MAC_ADDRESS                      *MCastFilter       OPTIONAL
);

///////////////////////////////////////////////////////////////////////////////
//

typedef
EFI_STATUS
(EFIAPI *EFI_SIMPLE_NETWORK_STATION_ADDRESS) (
    IN struct _EFI_SIMPLE_NETWORK_PROTOCOL  *This,
    IN bool                              Reset,
    IN EFI_MAC_ADDRESS                      *New      OPTIONAL
);

///////////////////////////////////////////////////////////////////////////////
//

typedef
EFI_STATUS
(EFIAPI *EFI_SIMPLE_NETWORK_STATISTICS) (
    IN struct _EFI_SIMPLE_NETWORK_PROTOCOL  *This,
    IN bool                              Reset,
    IN OUT uint64_t                            *StatisticsSize   OPTIONAL,
    OUT EFI_NETWORK_STATISTICS              *StatisticsTable  OPTIONAL
);

///////////////////////////////////////////////////////////////////////////////
//

typedef
EFI_STATUS
(EFIAPI *EFI_SIMPLE_NETWORK_MCAST_IP_TO_MAC) (
    IN struct _EFI_SIMPLE_NETWORK_PROTOCOL  *This,
    IN bool                              IPv6,
    IN EFI_IP_ADDRESS                       *IP,
    OUT EFI_MAC_ADDRESS                     *MAC
);

///////////////////////////////////////////////////////////////////////////////
//

typedef
EFI_STATUS
(EFIAPI *EFI_SIMPLE_NETWORK_NVDATA) (
    IN struct _EFI_SIMPLE_NETWORK_PROTOCOL  *This,
    IN bool                              ReadWrite,
    IN uint64_t                                Offset,
    IN uint64_t                                BufferSize,
    IN OUT void                             *Buffer
);

///////////////////////////////////////////////////////////////////////////////
//

typedef
EFI_STATUS
(EFIAPI *EFI_SIMPLE_NETWORK_GET_STATUS) (
    IN struct _EFI_SIMPLE_NETWORK_PROTOCOL  *This,
    OUT uint32_t                              *InterruptStatus  OPTIONAL,
    OUT void                                **TxBuf           OPTIONAL
);

///////////////////////////////////////////////////////////////////////////////
//

typedef
EFI_STATUS
(EFIAPI *EFI_SIMPLE_NETWORK_TRANSMIT) (
    IN struct _EFI_SIMPLE_NETWORK_PROTOCOL  *This,
    IN uint64_t                                HeaderSize,
    IN uint64_t                                BufferSize,
    IN void                                 *Buffer,
    IN EFI_MAC_ADDRESS                      *SrcAddr     OPTIONAL,
    IN EFI_MAC_ADDRESS                      *DestAddr    OPTIONAL,
    IN uint16_t                               *Protocol    OPTIONAL
);

///////////////////////////////////////////////////////////////////////////////
//

typedef
EFI_STATUS
(EFIAPI *EFI_SIMPLE_NETWORK_RECEIVE) (
    IN struct _EFI_SIMPLE_NETWORK_PROTOCOL  *This,
    OUT uint64_t                               *HeaderSize  OPTIONAL,
    IN OUT uint64_t                            *BufferSize,
    OUT void                                *Buffer,
    OUT EFI_MAC_ADDRESS                     *SrcAddr     OPTIONAL,
    OUT EFI_MAC_ADDRESS                     *DestAddr    OPTIONAL,
    OUT uint16_t                              *Protocol    OPTIONAL
);

///////////////////////////////////////////////////////////////////////////////
//

#define EFI_SIMPLE_NETWORK_PROTOCOL_REVISION  0x00010000
#define EFI_SIMPLE_NETWORK_INTERFACE_REVISION EFI_SIMPLE_NETWORK_PROTOCOL_REVISION

typedef struct _EFI_SIMPLE_NETWORK_PROTOCOL {
    uint64_t                              Revision;
    EFI_SIMPLE_NETWORK_START            Start;
    EFI_SIMPLE_NETWORK_STOP             Stop;
    EFI_SIMPLE_NETWORK_INITIALIZE       Initialize;
    EFI_SIMPLE_NETWORK_RESET            Reset;
    EFI_SIMPLE_NETWORK_SHUTDOWN         Shutdown;
    EFI_SIMPLE_NETWORK_RECEIVE_FILTERS  ReceiveFilters;
    EFI_SIMPLE_NETWORK_STATION_ADDRESS  StationAddress;
    EFI_SIMPLE_NETWORK_STATISTICS       Statistics;
    EFI_SIMPLE_NETWORK_MCAST_IP_TO_MAC  MCastIpToMac;
    EFI_SIMPLE_NETWORK_NVDATA           NvData;
    EFI_SIMPLE_NETWORK_GET_STATUS       GetStatus;
    EFI_SIMPLE_NETWORK_TRANSMIT         Transmit;
    EFI_SIMPLE_NETWORK_RECEIVE          Receive;
    EFI_EVENT                           WaitForPacket;
    EFI_SIMPLE_NETWORK_MODE             *Mode;
} EFI_SIMPLE_NETWORK_PROTOCOL;

// Note: Because it conflicted with the EDK2 struct name, the
// 'EFI_SIMPLE_NETWORK_PROTOCOL' GUID definition, from older
// versions of gnu-efi, is now obsoleted.
// Use 'EFI_SIMPLE_NETWORK_PROTOCOL_GUID' instead.

typedef struct _EFI_SIMPLE_NETWORK_PROTOCOL _EFI_SIMPLE_NETWORK;
typedef EFI_SIMPLE_NETWORK_PROTOCOL EFI_SIMPLE_NETWORK;

#endif /* _EFINET_H */
