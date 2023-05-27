#ifndef _EFIPXEBC_H
#define _EFIPXEBC_H

/*++

Copyright (c) 1998  Intel Corporation

Module Name:

    efipxebc.h

Abstract:

    EFI PXE Base Code Protocol



Revision History

--*/

//
// PXE Base Code protocol
//

#define EFI_PXE_BASE_CODE_PROTOCOL_GUID \
    { 0x03c4e603, 0xac28, 0x11d3, {0x9a, 0x2d, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d} }

INTERFACE_DECL(_EFI_PXE_BASE_CODE_PROTOCOL);

#define DEFAULT_TTL 4
#define DEFAULT_ToS 0
//
// Address definitions
//

typedef union {
    uint32_t      Addr[4];
    EFI_IPv4_ADDRESS    v4;
    EFI_IPv6_ADDRESS    v6;
} EFI_IP_ADDRESS;

typedef uint16_t EFI_PXE_BASE_CODE_UDP_PORT;

//
// Packet definitions
//

typedef struct {
    uint8_t                           BootpOpcode;
    uint8_t                           BootpHwType;
    uint8_t                           BootpHwAddrLen;
    uint8_t                           BootpGateHops;
    uint32_t                          BootpIdent;
    uint16_t                          BootpSeconds;
    uint16_t                          BootpFlags;
    uint8_t                           BootpCiAddr[4];
    uint8_t                           BootpYiAddr[4];
    uint8_t                           BootpSiAddr[4];
    uint8_t                           BootpGiAddr[4];
    uint8_t                           BootpHwAddr[16];
    uint8_t                           BootpSrvName[64];
    uint8_t                           BootpBootFile[128];
    uint32_t                          DhcpMagik;
    uint8_t                           DhcpOptions[56];
} EFI_PXE_BASE_CODE_DHCPV4_PACKET;

typedef struct {
    uint32_t                          MessageType:8;
    uint32_t                          TransactionId:24;
    uint8_t                           DhcpOptions[1024];
} EFI_PXE_BASE_CODE_DHCPV6_PACKET;

typedef union {
    uint8_t                               Raw[1472];
    EFI_PXE_BASE_CODE_DHCPV4_PACKET     Dhcpv4;
    EFI_PXE_BASE_CODE_DHCPV6_PACKET     Dhcpv6;
} EFI_PXE_BASE_CODE_PACKET;

typedef struct {
    uint8_t                   Type;
    uint8_t                   Code;
    uint16_t                  Checksum;
    union {
        uint32_t              reserved;
        uint32_t              Mtu;
        uint32_t              Pointer;
        struct {
            uint16_t          Identifier;
            uint16_t          Sequence;
        } Echo;
    } u;
    uint8_t                   Data[494];
} EFI_PXE_BASE_CODE_ICMP_ERROR;

typedef struct {
    uint8_t                   ErrorCode;
    const char                   ErrorString[127];
} EFI_PXE_BASE_CODE_TFTP_ERROR;

//
// IP Receive Filter definitions
//
#define EFI_PXE_BASE_CODE_MAX_IPCNT             8
typedef struct {
    uint8_t                       Filters;
    uint8_t                       IpCnt;
    uint16_t                      reserved;
    EFI_IP_ADDRESS              IpList[EFI_PXE_BASE_CODE_MAX_IPCNT];
} EFI_PXE_BASE_CODE_IP_FILTER;

#define EFI_PXE_BASE_CODE_IP_FILTER_STATION_IP             0x0001
#define EFI_PXE_BASE_CODE_IP_FILTER_BROADCAST              0x0002
#define EFI_PXE_BASE_CODE_IP_FILTER_PROMISCUOUS            0x0004
#define EFI_PXE_BASE_CODE_IP_FILTER_PROMISCUOUS_MULTICAST  0x0008

//
// ARP Cache definitions
//

typedef struct {
    EFI_IP_ADDRESS       IpAddr;
    EFI_MAC_ADDRESS      MacAddr;
} EFI_PXE_BASE_CODE_ARP_ENTRY;

typedef struct {
    EFI_IP_ADDRESS       IpAddr;
    EFI_IP_ADDRESS       SubnetMask;
    EFI_IP_ADDRESS       GwAddr;
} EFI_PXE_BASE_CODE_ROUTE_ENTRY;

//
// UDP definitions
//

#define EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_SRC_IP    0x0001
#define EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_SRC_PORT  0x0002
#define EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_DEST_IP   0x0004
#define EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_DEST_PORT 0x0008
#define EFI_PXE_BASE_CODE_UDP_OPFLAGS_USE_FILTER    0x0010
#define EFI_PXE_BASE_CODE_UDP_OPFLAGS_MAY_FRAGMENT  0x0020

//
// Discover() definitions
//

#define EFI_PXE_BASE_CODE_BOOT_TYPE_BOOTSTRAP           0
#define EFI_PXE_BASE_CODE_BOOT_TYPE_MS_WINNT_RIS        1
#define EFI_PXE_BASE_CODE_BOOT_TYPE_INTEL_LCM           2
#define EFI_PXE_BASE_CODE_BOOT_TYPE_DOSUNDI             3
#define EFI_PXE_BASE_CODE_BOOT_TYPE_NEC_ESMPRO          4
#define EFI_PXE_BASE_CODE_BOOT_TYPE_IBM_WSoD            5
#define EFI_PXE_BASE_CODE_BOOT_TYPE_IBM_LCCM            6
#define EFI_PXE_BASE_CODE_BOOT_TYPE_CA_UNICENTER_TNG    7
#define EFI_PXE_BASE_CODE_BOOT_TYPE_HP_OPENVIEW         8
#define EFI_PXE_BASE_CODE_BOOT_TYPE_ALTIRIS_9           9
#define EFI_PXE_BASE_CODE_BOOT_TYPE_ALTIRIS_10          10
#define EFI_PXE_BASE_CODE_BOOT_TYPE_ALTIRIS_11          11
#define EFI_PXE_BASE_CODE_BOOT_TYPE_NOT_USED_12         12
#define EFI_PXE_BASE_CODE_BOOT_TYPE_REDHAT_INSTALL      13
#define EFI_PXE_BASE_CODE_BOOT_TYPE_REDHAT_BOOT         14
#define EFI_PXE_BASE_CODE_BOOT_TYPE_REMBO               15
#define EFI_PXE_BASE_CODE_BOOT_TYPE_BEOBOOT             16
//
// 17 through 32767 are reserved
// 32768 through 65279 are for vendor use
// 65280 through 65534 are reserved
//
#define EFI_PXE_BASE_CODE_BOOT_TYPE_PXETEST             65535

#define EFI_PXE_BASE_CODE_BOOT_LAYER_MASK               0x7FFF
#define EFI_PXE_BASE_CODE_BOOT_LAYER_INITIAL            0x0000


typedef struct {
    uint16_t                      Type;
    bool                     AcceptAnyResponse;
    uint8_t                       Reserved;
    EFI_IP_ADDRESS              IpAddr;
} EFI_PXE_BASE_CODE_SRVLIST;

typedef struct {
    bool                     UseMCast;
    bool                     UseBCast;
    bool                     UseUCast;
    bool                     MustUseList;
    EFI_IP_ADDRESS              ServerMCastIp;
    uint16_t                      IpCnt;
    EFI_PXE_BASE_CODE_SRVLIST   SrvList[1];
} EFI_PXE_BASE_CODE_DISCOVER_INFO;

//
// Mtftp() definitions
//

typedef enum {
    EFI_PXE_BASE_CODE_TFTP_FIRST,
    EFI_PXE_BASE_CODE_TFTP_GET_FILE_SIZE,
    EFI_PXE_BASE_CODE_TFTP_READ_FILE,
    EFI_PXE_BASE_CODE_TFTP_WRITE_FILE,
    EFI_PXE_BASE_CODE_TFTP_READ_DIRECTORY,
    EFI_PXE_BASE_CODE_MTFTP_GET_FILE_SIZE,
    EFI_PXE_BASE_CODE_MTFTP_READ_FILE,
    EFI_PXE_BASE_CODE_MTFTP_READ_DIRECTORY,
    EFI_PXE_BASE_CODE_MTFTP_LAST
} EFI_PXE_BASE_CODE_TFTP_OPCODE;

typedef struct {
    EFI_IP_ADDRESS   MCastIp;
    EFI_PXE_BASE_CODE_UDP_PORT  CPort;
    EFI_PXE_BASE_CODE_UDP_PORT  SPort;
    uint16_t                      ListenTimeout;
    uint16_t                      TransmitTimeout;
} EFI_PXE_BASE_CODE_MTFTP_INFO;

//
// PXE Base Code Mode structure
//

#define EFI_PXE_BASE_CODE_MAX_ARP_ENTRIES       8
#define EFI_PXE_BASE_CODE_MAX_ROUTE_ENTRIES     8

typedef struct {
    bool                         Started;
    bool                         Ipv6Available;
    bool                         Ipv6Supported;
    bool                         UsingIpv6;
    bool                         BisSupported;
    bool                         BisDetected;
    bool                         AutoArp;
    bool                         SendGUID;
    bool                         DhcpDiscoverValid;
    bool                         DhcpAckReceived;
    bool                         ProxyOfferReceived;
    bool                         PxeDiscoverValid;
    bool                         PxeReplyReceived;
    bool                         PxeBisReplyReceived;
    bool                         IcmpErrorReceived;
    bool                         TftpErrorReceived;
    bool                         MakeCallbacks;
    uint8_t                           TTL;
    uint8_t                           ToS;
    EFI_IP_ADDRESS                  StationIp;
    EFI_IP_ADDRESS                  SubnetMask;
    EFI_PXE_BASE_CODE_PACKET        DhcpDiscover;
    EFI_PXE_BASE_CODE_PACKET        DhcpAck;
    EFI_PXE_BASE_CODE_PACKET        ProxyOffer;
    EFI_PXE_BASE_CODE_PACKET        PxeDiscover;
    EFI_PXE_BASE_CODE_PACKET        PxeReply;
    EFI_PXE_BASE_CODE_PACKET        PxeBisReply;
    EFI_PXE_BASE_CODE_IP_FILTER     IpFilter;
    uint32_t                          ArpCacheEntries;
    EFI_PXE_BASE_CODE_ARP_ENTRY     ArpCache[EFI_PXE_BASE_CODE_MAX_ARP_ENTRIES];
    uint32_t                          RouteTableEntries;
    EFI_PXE_BASE_CODE_ROUTE_ENTRY   RouteTable[EFI_PXE_BASE_CODE_MAX_ROUTE_ENTRIES];
    EFI_PXE_BASE_CODE_ICMP_ERROR    IcmpError;
    EFI_PXE_BASE_CODE_TFTP_ERROR    TftpError;
} EFI_PXE_BASE_CODE_MODE;

//
// PXE Base Code Interface Function definitions
//

typedef
EFI_STATUS
(EFIAPI *EFI_PXE_BASE_CODE_START) (
    IN struct _EFI_PXE_BASE_CODE_PROTOCOL  *This,
    IN bool                             UseIpv6
    );

typedef
EFI_STATUS
(EFIAPI *EFI_PXE_BASE_CODE_STOP) (
    IN struct _EFI_PXE_BASE_CODE_PROTOCOL  *This
    );

typedef
EFI_STATUS
(EFIAPI *EFI_PXE_BASE_CODE_DHCP) (
    IN struct _EFI_PXE_BASE_CODE_PROTOCOL  *This,
    IN bool                             SortOffers
    );

typedef
EFI_STATUS
(EFIAPI *EFI_PXE_BASE_CODE_DISCOVER) (
    IN struct _EFI_PXE_BASE_CODE_PROTOCOL   *This,
    IN uint16_t                               Type,
    IN uint16_t                               *Layer,
    IN bool                              UseBis,
    IN OUT EFI_PXE_BASE_CODE_DISCOVER_INFO  *Info   OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_PXE_BASE_CODE_MTFTP) (
    IN struct _EFI_PXE_BASE_CODE_PROTOCOL  *This,
    IN EFI_PXE_BASE_CODE_TFTP_OPCODE       Operation,
    IN OUT void                            *BufferPtr  OPTIONAL,
    IN bool                             Overwrite,
    IN OUT uint64_t                          *BufferSize,
    IN uint64_t                               *BlockSize  OPTIONAL,
    IN EFI_IP_ADDRESS                      *ServerIp,
    IN uint8_t                               *Filename,
    IN EFI_PXE_BASE_CODE_MTFTP_INFO        *Info       OPTIONAL,
    IN bool                             DontUseBuffer
    );

typedef
EFI_STATUS
(EFIAPI *EFI_PXE_BASE_CODE_UDP_WRITE) (
    IN struct _EFI_PXE_BASE_CODE_PROTOCOL  *This,
    IN uint16_t                              OpFlags,
    IN EFI_IP_ADDRESS                      *DestIp,
    IN EFI_PXE_BASE_CODE_UDP_PORT          *DestPort,
    IN EFI_IP_ADDRESS                      *GatewayIp,  OPTIONAL
    IN EFI_IP_ADDRESS                      *SrcIp,      OPTIONAL
    IN OUT EFI_PXE_BASE_CODE_UDP_PORT      *SrcPort,    OPTIONAL
    IN uint64_t                               *HeaderSize, OPTIONAL
    IN void                                *HeaderPtr,  OPTIONAL
    IN uint64_t                               *BufferSize,
    IN void                                *BufferPtr
    );

typedef
EFI_STATUS
(EFIAPI *EFI_PXE_BASE_CODE_UDP_READ) (
    IN struct _EFI_PXE_BASE_CODE_PROTOCOL  *This,
    IN uint16_t                              OpFlags,
    IN OUT EFI_IP_ADDRESS                  *DestIp,      OPTIONAL
    IN OUT EFI_PXE_BASE_CODE_UDP_PORT      *DestPort,    OPTIONAL
    IN OUT EFI_IP_ADDRESS                  *SrcIp,       OPTIONAL
    IN OUT EFI_PXE_BASE_CODE_UDP_PORT      *SrcPort,     OPTIONAL
    IN uint64_t                               *HeaderSize,  OPTIONAL
    IN void                                *HeaderPtr,   OPTIONAL
    IN OUT uint64_t                           *BufferSize,
    IN void                                *BufferPtr
    );

typedef
EFI_STATUS
(EFIAPI *EFI_PXE_BASE_CODE_SET_IP_FILTER) (
    IN struct _EFI_PXE_BASE_CODE_PROTOCOL  *This,
    IN EFI_PXE_BASE_CODE_IP_FILTER         *NewFilter
    );

typedef
EFI_STATUS
(EFIAPI *EFI_PXE_BASE_CODE_ARP) (
    IN struct _EFI_PXE_BASE_CODE_PROTOCOL  *This,
    IN EFI_IP_ADDRESS                      *IpAddr,
    IN EFI_MAC_ADDRESS                     *MacAddr      OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_PXE_BASE_CODE_SET_PARAMETERS) (
    IN struct _EFI_PXE_BASE_CODE_PROTOCOL  *This,
    IN bool                             *NewAutoArp,    OPTIONAL
    IN bool                             *NewSendGUID,   OPTIONAL
    IN uint8_t                               *NewTTL,        OPTIONAL
    IN uint8_t                               *NewToS,        OPTIONAL
    IN bool                             *NewMakeCallback    OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_PXE_BASE_CODE_SET_STATION_IP) (
    IN struct _EFI_PXE_BASE_CODE_PROTOCOL  *This,
    IN EFI_IP_ADDRESS                      *NewStationIp,  OPTIONAL
    IN EFI_IP_ADDRESS                      *NewSubnetMask  OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_PXE_BASE_CODE_SET_PACKETS) (
    IN struct _EFI_PXE_BASE_CODE_PROTOCOL  *This,
    bool                                *NewDhcpDiscoverValid,  OPTIONAL
    bool                                *NewDhcpAckReceived,    OPTIONAL
    bool                                *NewProxyOfferReceived, OPTIONAL
    bool                                *NewPxeDiscoverValid,   OPTIONAL
    bool                                *NewPxeReplyReceived,   OPTIONAL
    bool                                *NewPxeBisReplyReceived,OPTIONAL
    IN EFI_PXE_BASE_CODE_PACKET            *NewDhcpDiscover, OPTIONAL
    IN EFI_PXE_BASE_CODE_PACKET            *NewDhcpAck,      OPTIONAL
    IN EFI_PXE_BASE_CODE_PACKET            *NewProxyOffer,   OPTIONAL
    IN EFI_PXE_BASE_CODE_PACKET            *NewPxeDiscover,  OPTIONAL
    IN EFI_PXE_BASE_CODE_PACKET            *NewPxeReply,     OPTIONAL
    IN EFI_PXE_BASE_CODE_PACKET            *NewPxeBisReply   OPTIONAL
    );

//
// PXE Base Code Protocol structure
//

#define EFI_PXE_BASE_CODE_PROTOCOL_REVISION  0x00010000
#define EFI_PXE_BASE_CODE_INTERFACE_REVISION EFI_PXE_BASE_CODE_PROTOCOL_REVISION

typedef struct _EFI_PXE_BASE_CODE_PROTOCOL {
    uint64_t                              Revision;
    EFI_PXE_BASE_CODE_START             Start;
    EFI_PXE_BASE_CODE_STOP              Stop;
    EFI_PXE_BASE_CODE_DHCP              Dhcp;
    EFI_PXE_BASE_CODE_DISCOVER          Discover;
    EFI_PXE_BASE_CODE_MTFTP             Mtftp;
    EFI_PXE_BASE_CODE_UDP_WRITE         UdpWrite;
    EFI_PXE_BASE_CODE_UDP_READ          UdpRead;
    EFI_PXE_BASE_CODE_SET_IP_FILTER     SetIpFilter;
    EFI_PXE_BASE_CODE_ARP               Arp;
    EFI_PXE_BASE_CODE_SET_PARAMETERS    SetParameters;
    EFI_PXE_BASE_CODE_SET_STATION_IP    SetStationIp;
    EFI_PXE_BASE_CODE_SET_PACKETS       SetPackets;
    EFI_PXE_BASE_CODE_MODE              *Mode;
} EFI_PXE_BASE_CODE_PROTOCOL;

// Note: Because it conflicted with the EDK2 struct name, the
// 'EFI_PXE_BASE_CODE_PROTOCOL' GUID definition, from older
// versions of gnu-efi, is now obsoleted.
// Use 'EFI_PXE_BASE_CODE_PROTOCOL_GUID' instead.

typedef struct _EFI_PXE_BASE_CODE_PROTOCOL _EFI_PXE_BASE_CODE;
typedef struct _EFI_PXE_BASE_CODE_PROTOCOL EFI_PXE_BASE_CODE;

//
// Call Back Definitions
//

#define EFI_PXE_BASE_CODE_CALLBACK_PROTOCOL_GUID \
    { 0x245dca21, 0xfb7b, 0x11d3, {0x8f, 0x01, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b} }

//
// Revision Number
//

#define EFI_PXE_BASE_CODE_CALLBACK_PROTOCOL_REVISION  0x00010000
#define EFI_PXE_BASE_CODE_CALLBACK_INTERFACE_REVISION EFI_PXE_BASE_CODE_CALLBACK_PROTOCOL_REVISION

INTERFACE_DECL(_EFI_PXE_BASE_CODE_CALLBACK_PROTOCOL);

typedef enum {
    EFI_PXE_BASE_CODE_FUNCTION_FIRST,
    EFI_PXE_BASE_CODE_FUNCTION_DHCP,
    EFI_PXE_BASE_CODE_FUNCTION_DISCOVER,
    EFI_PXE_BASE_CODE_FUNCTION_MTFTP,
    EFI_PXE_BASE_CODE_FUNCTION_UDP_WRITE,
    EFI_PXE_BASE_CODE_FUNCTION_UDP_READ,
    EFI_PXE_BASE_CODE_FUNCTION_ARP,
    EFI_PXE_BASE_CODE_FUNCTION_IGMP,
    EFI_PXE_BASE_CODE_PXE_FUNCTION_LAST
} EFI_PXE_BASE_CODE_FUNCTION;

typedef enum {
    EFI_PXE_BASE_CODE_CALLBACK_STATUS_FIRST,
    EFI_PXE_BASE_CODE_CALLBACK_STATUS_CONTINUE,
    EFI_PXE_BASE_CODE_CALLBACK_STATUS_ABORT,
    EFI_PXE_BASE_CODE_CALLBACK_STATUS_LAST
} EFI_PXE_BASE_CODE_CALLBACK_STATUS;

typedef
EFI_PXE_BASE_CODE_CALLBACK_STATUS
(EFIAPI *EFI_PXE_CALLBACK) (
    IN struct _EFI_PXE_BASE_CODE_CALLBACK_PROTOCOL  *This,
    IN EFI_PXE_BASE_CODE_FUNCTION                   Function,
    IN bool                                      Received,
    IN uint32_t                                       PacketLen,
    IN EFI_PXE_BASE_CODE_PACKET                     *Packet     OPTIONAL
    );

typedef struct _EFI_PXE_BASE_CODE_CALLBACK_PROTOCOL {
    uint64_t                      Revision;
    EFI_PXE_CALLBACK            Callback;
} EFI_PXE_BASE_CODE_CALLBACK_PROTOCOL;

// Note: Because it conflicted with the EDK2 struct name, the
// 'EFI_PXE_BASE_CODE_CALLBACK_PROTOCOL' GUID definition, from
// older versions of gnu-efi, is now obsoleted.
// Use 'EFI_PXE_BASE_CODE_CALLBACK_PROTOCOL_GUID' instead.

typedef struct _EFI_PXE_BASE_CODE_CALLBACK_PROTOCOL _EFI_PXE_BASE_CODE_CALLBACK;
typedef EFI_PXE_BASE_CODE_CALLBACK_PROTOCOL EFI_PXE_BASE_CODE_CALLBACK;

#endif /* _EFIPXEBC_H */
