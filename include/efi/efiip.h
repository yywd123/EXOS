#ifndef _EFI_IP_H
#define _EFI_IP_H

/*++
Copyright (c) 2013  Intel Corporation

--*/

#define EFI_IP4_SERVICE_BINDING_PROTOCOL \
   {0xc51711e7,0xb4bf,0x404a,{0xbf,0xb8,0x0a,0x04, 0x8e,0xf1,0xff,0xe4}}

#define EFI_IP4_PROTOCOL \
    {0x41d94cd2,0x35b6,0x455a,{0x82,0x58,0xd4,0xe5,0x13,0x34,0xaa,0xdd}}

#define EFI_IP6_SERVICE_BINDING_PROTOCOL \
    {0xec835dd3,0xfe0f,0x617b,{0xa6,0x21,0xb3,0x50,0xc3,0xe1,0x33,0x88}}

#define EFI_IP6_PROTOCOL \
    {0x2c8759d5,0x5c2d,0x66ef,{0x92,0x5f,0xb6,0x6c,0x10,0x19,0x57,0xe2}}

INTERFACE_DECL(_EFI_IP4);
INTERFACE_DECL(_EFI_IP6);

typedef struct {
    EFI_HANDLE       InstanceHandle;
    EFI_IPv4_ADDRESS Ip4Address;
    EFI_IPv4_ADDRESS SubnetMask;
} EFI_IP4_ADDRESS_PAIR;

typedef struct {
    EFI_HANDLE           DriverHandle;
    uint32_t               AddressCount;
    EFI_IP4_ADDRESS_PAIR AddressPairs[1];
} EFI_IP4_VARIABLE_DATA;

typedef struct {
    uint8_t            DefaultProtocol;
    bool          AcceptAnyProtocol;
    bool          AcceptIcmpErrors;
    bool          AcceptBroadcast;
    bool          AcceptPromiscuous;
    bool          UseDefaultAddress;
    EFI_IPv4_ADDRESS StationAddress;
    EFI_IPv4_ADDRESS SubnetMask;
    uint8_t            TypeOfService;
    uint8_t            TimeToLive;
    bool          DoNotFragment;
    bool          RawData;
    uint32_t           ReceiveTimeout;
    uint32_t           TransmitTimeout;
} EFI_IP4_CONFIG_DATA;

typedef struct {
    EFI_IPv4_ADDRESS SubnetAddress;
    EFI_IPv4_ADDRESS SubnetMask;
    EFI_IPv4_ADDRESS GatewayAddress;
} EFI_IP4_ROUTE_TABLE;

typedef struct {
    uint8_t Type;
    uint8_t Code;
} EFI_IP4_ICMP_TYPE;

typedef struct {
    bool             IsStarted;
    uint32_t              MaxPacketSize;
    EFI_IP4_CONFIG_DATA ConfigData;
    bool             IsConfigured;
    uint32_t              GroupCount;
    EFI_IPv4_ADDRESS    *GroupTable;
    uint32_t              RouteCount;
    EFI_IP4_ROUTE_TABLE *RouteTable;
    uint32_t              IcmpTypeCount;
    EFI_IP4_ICMP_TYPE   *IcmpTypeList;
} EFI_IP4_MODE_DATA;

typedef
EFI_STATUS
(EFIAPI *EFI_IP4_GET_MODE_DATA) (
    IN struct _EFI_IP4                  *This,
    OUT EFI_IP4_MODE_DATA               *Ip4ModeData   OPTIONAL,
    OUT EFI_MANAGED_NETWORK_CONFIG_DATA *MnpConfigData OPTIONAL,
    OUT EFI_SIMPLE_NETWORK_MODE         *SnpModeData   OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_IP4_CONFIGURE) (
    IN struct _EFI_IP4     *This,
    IN EFI_IP4_CONFIG_DATA *IpConfigData OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_IP4_GROUPS) (
    IN struct _EFI_IP4  *This,
    IN bool          JoinFlag,
    IN EFI_IPv4_ADDRESS *GroupAddress OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_IP4_ROUTES) (
    IN struct _EFI_IP4  *This,
    IN bool          DeleteRoute,
    IN EFI_IPv4_ADDRESS *SubnetAddress,
    IN EFI_IPv4_ADDRESS *SubnetMask,
    IN EFI_IPv4_ADDRESS *GatewayAddress
    );

#pragma pack(1)
typedef struct {
    uint8_t            HeaderLength:4;
    uint8_t            Version:4;
    uint8_t            TypeOfService;
    uint16_t           TotalLength;
    uint16_t           Identification;
    uint16_t           Fragmentation;
    uint8_t            TimeToLive;
    uint8_t            Protocol;
    uint16_t           Checksum;
    EFI_IPv4_ADDRESS SourceAddress;
    EFI_IPv4_ADDRESS DestinationAddress;
} EFI_IP4_HEADER;
#pragma pack()

typedef struct {
    uint32_t FragmentLength;
    void   *FragmentBuffer;
} EFI_IP4_FRAGMENT_DATA;

typedef struct {
    EFI_TIME              TimeStamp;
    EFI_EVENT             RecycleSignal;
    uint32_t                HeaderLength;
    EFI_IP4_HEADER        *Header;
    uint32_t                OptionsLength;
    void                  *Options;
    uint32_t                DataLength;
    uint32_t                FragmentCount;
    EFI_IP4_FRAGMENT_DATA FragmentTable[1];
} EFI_IP4_RECEIVE_DATA;

typedef struct {
    EFI_IPv4_ADDRESS SourceAddress;
    EFI_IPv4_ADDRESS GatewayAddress;
    uint8_t            Protocol;
    uint8_t            TypeOfService;
    uint8_t            TimeToLive;
    bool          DoNotFragment;
} EFI_IP4_OVERRIDE_DATA;

typedef struct {
    EFI_IPv4_ADDRESS      DestinationAddress;
    EFI_IP4_OVERRIDE_DATA *OverrideData;
    uint32_t                OptionsLength;
    void                  *OptionsBuffer;
    uint32_t                TotalDataLength;
    uint32_t                FragmentCount;
    EFI_IP4_FRAGMENT_DATA FragmentTable[1];
} EFI_IP4_TRANSMIT_DATA;

typedef struct {
    EFI_EVENT                 Event;
    EFI_STATUS                Status;
    union {
        EFI_IP4_RECEIVE_DATA  *RxData;
        EFI_IP4_TRANSMIT_DATA *TxData;
    } Packet;
} EFI_IP4_COMPLETION_TOKEN;

typedef
EFI_STATUS
(EFIAPI *EFI_IP4_TRANSMIT) (
    IN struct _EFI_IP4          *This,
    IN EFI_IP4_COMPLETION_TOKEN *Token
    );

typedef
EFI_STATUS
(EFIAPI *EFI_IP4_RECEIVE) (
    IN struct _EFI_IP4          *This,
    IN EFI_IP4_COMPLETION_TOKEN *Token
    );

typedef
EFI_STATUS
(EFIAPI *EFI_IP4_CANCEL)(
    IN struct _EFI_IP4          *This,
    IN EFI_IP4_COMPLETION_TOKEN *Token OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_IP4_POLL) (
    IN struct _EFI_IP4 *This
    );

typedef struct _EFI_IP4 {
    EFI_IP4_GET_MODE_DATA GetModeData;
    EFI_IP4_CONFIGURE     Configure;
    EFI_IP4_GROUPS        Groups;
    EFI_IP4_ROUTES        Routes;
    EFI_IP4_TRANSMIT      Transmit;
    EFI_IP4_RECEIVE       Receive;
    EFI_IP4_CANCEL        Cancel;
    EFI_IP4_POLL          Poll;
} EFI_IP4;

typedef struct {
    uint8_t            DefaultProtocol;
    bool          AcceptAnyProtocol;
    bool          AcceptIcmpErrors;
    bool          AcceptPromiscuous;
    EFI_IPv6_ADDRESS DestinationAddress;
    EFI_IPv6_ADDRESS StationAddress;
    uint8_t            TrafficClass;
    uint8_t            HopLimit;
    uint32_t           FlowLabel;
    uint32_t           ReceiveTimeout;
    uint32_t           TransmitTimeout;
} EFI_IP6_CONFIG_DATA;

typedef struct {
    EFI_IPv6_ADDRESS Address;
    uint8_t            PrefixLength;
} EFI_IP6_ADDRESS_INFO;

typedef struct {
    EFI_IPv6_ADDRESS Gateway;
    EFI_IPv6_ADDRESS Destination;
    uint8_t            PrefixLength;
} EFI_IP6_ROUTE_TABLE;

typedef enum {
    EfiNeighborInComplete,
    EfiNeighborReachable,
    EfiNeighborStale,
    EfiNeighborDelay,
    EfiNeighborProbe
} EFI_IP6_NEIGHBOR_STATE;

typedef struct {
    EFI_IPv6_ADDRESS       Neighbor;
    EFI_MAC_ADDRESS        LinkAddress;
    EFI_IP6_NEIGHBOR_STATE State;
} EFI_IP6_NEIGHBOR_CACHE;

typedef struct {
    uint8_t Type;
    uint8_t Code;
} EFI_IP6_ICMP_TYPE;

//***********************************************************
// ICMPv6 type definitions for error messages
//***********************************************************
#define ICMP_V6_DEST_UNREACHABLE     0x1
#define ICMP_V6_PACKET_TOO_BIG       0x2
#define ICMP_V6_TIME_EXCEEDED        0x3
#define ICMP_V6_PARAMETER_PROBLEM    0x4

//***********************************************************
// ICMPv6 type definition for informational messages
//***********************************************************
#define ICMP_V6_ECHO_REQUEST         0x80
#define ICMP_V6_ECHO_REPLY           0x81
#define ICMP_V6_LISTENER_QUERY       0x82
#define ICMP_V6_LISTENER_REPORT      0x83
#define ICMP_V6_LISTENER_DONE        0x84
#define ICMP_V6_ROUTER_SOLICIT       0x85
#define ICMP_V6_ROUTER_ADVERTISE     0x86
#define ICMP_V6_NEIGHBOR_SOLICIT     0x87
#define ICMP_V6_NEIGHBOR_ADVERTISE   0x88
#define ICMP_V6_REDIRECT             0x89
#define ICMP_V6_LISTENER_REPORT_2    0x8F

//***********************************************************
// ICMPv6 code definitions for ICMP_V6_DEST_UNREACHABLE
//***********************************************************
#define ICMP_V6_NO_ROUTE_TO_DEST     0x0
#define ICMP_V6_COMM_PROHIBITED      0x1
#define ICMP_V6_BEYOND_SCOPE         0x2
#define ICMP_V6_ADDR_UNREACHABLE     0x3
#define ICMP_V6_PORT_UNREACHABLE     0x4
#define ICMP_V6_SOURCE_ADDR_FAILED   0x5
#define ICMP_V6_ROUTE_REJECTED       0x6

//***********************************************************
// ICMPv6 code definitions for ICMP_V6_TIME_EXCEEDED
//***********************************************************
#define ICMP_V6_TIMEOUT_HOP_LIMIT    0x0
#define ICMP_V6_TIMEOUT_REASSEMBLE   0x1

//***********************************************************
// ICMPv6 code definitions for ICMP_V6_PARAMETER_PROBLEM
//***********************************************************
#define ICMP_V6_ERRONEOUS_HEADER     0x0
#define ICMP_V6_UNRECOGNIZE_NEXT_HDR 0x1
#define ICMP_V6_UNRECOGNIZE_OPTION   0x2

typedef struct {
    bool                IsStarted;
    uint32_t                 MaxPacketSize;
    EFI_IP6_CONFIG_DATA    ConfigData;
    bool                IsConfigured;
    uint32_t                 AddressCount;
    EFI_IP6_ADDRESS_INFO   *AddressList;
    uint32_t                 GroupCount;
    EFI_IPv6_ADDRESS       *GroupTable;
    uint32_t                 RouteCount;
    EFI_IP6_ROUTE_TABLE    *RouteTable;
    uint32_t                 NeighborCount;
    EFI_IP6_NEIGHBOR_CACHE *NeighborCache;
    uint32_t                 PrefixCount;
    EFI_IP6_ADDRESS_INFO   *PrefixTable;
    uint32_t                 IcmpTypeCount;
    EFI_IP6_ICMP_TYPE      *IcmpTypeList;
} EFI_IP6_MODE_DATA;

typedef
EFI_STATUS
(EFIAPI *EFI_IP6_GET_MODE_DATA) (
    IN struct _EFI_IP6                  *This,
    OUT EFI_IP6_MODE_DATA               *Ip6ModeData   OPTIONAL,
    OUT EFI_MANAGED_NETWORK_CONFIG_DATA *MnpConfigData OPTIONAL,
    OUT EFI_SIMPLE_NETWORK_MODE         *SnpModeData   OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_IP6_CONFIGURE) (
    IN struct _EFI_IP6     *This,
    IN EFI_IP6_CONFIG_DATA *Ip6ConfigData OPTIONAL
    );
typedef
EFI_STATUS
(EFIAPI *EFI_IP6_GROUPS) (
    IN struct _EFI_IP6  *This,
    IN bool          JoinFlag,
    IN EFI_IPv6_ADDRESS *GroupAddress OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_IP6_ROUTES) (
    IN struct _EFI_IP6  *This,
    IN bool          DeleteRoute,
    IN EFI_IPv6_ADDRESS *Destination    OPTIONAL,
    IN uint8_t            PrefixLength,
    IN EFI_IPv6_ADDRESS *GatewayAddress OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_IP6_NEIGHBORS) (
    IN struct _EFI_IP6  *This,
    IN bool          DeleteFlag,
    IN EFI_IPv6_ADDRESS *TargetIp6Address,
    IN EFI_MAC_ADDRESS  *TargetLinkAddress OPTIONAL,
    IN uint32_t           Timeout,
    IN bool          Override
    );

typedef struct _EFI_IP6_FRAGMENT_DATA {
    uint32_t FragmentLength;
    void   *FragmentBuffer;
} EFI_IP6_FRAGMENT_DATA;

typedef struct _EFI_IP6_OVERRIDE_DATA {
    uint8_t  Protocol;
    uint8_t  HopLimit;
    uint32_t FlowLabel;
} EFI_IP6_OVERRIDE_DATA;

typedef struct _EFI_IP6_TRANSMIT_DATA {
    EFI_IPv6_ADDRESS      DestinationAddress;
    EFI_IP6_OVERRIDE_DATA *OverrideData;
    uint32_t                ExtHdrsLength;
    void                  *ExtHdrs;
    uint8_t                 NextHeader;
    uint32_t                DataLength;
    uint32_t                FragmentCount;
    EFI_IP6_FRAGMENT_DATA FragmentTable[1];
} EFI_IP6_TRANSMIT_DATA;

#pragma pack(1)
typedef struct _EFI_IP6_HEADER {
    uint8_t            TrafficClassH:4;
    uint8_t            Version:4;
    uint8_t            FlowLabelH:4;
    uint8_t            TrafficClassL:4;
    uint16_t           FlowLabelL;
    uint16_t           PayloadLength;
    uint8_t            NextHeader;
    uint8_t            HopLimit;
    EFI_IPv6_ADDRESS SourceAddress;
    EFI_IPv6_ADDRESS DestinationAddress;
} EFI_IP6_HEADER;
#pragma pack()

typedef struct _EFI_IP6_RECEIVE_DATA {
    EFI_TIME              TimeStamp;
    EFI_EVENT             RecycleSignal;
    uint32_t                HeaderLength;
    EFI_IP6_HEADER        *Header;
    uint32_t                DataLength;
    uint32_t                FragmentCount;
    EFI_IP6_FRAGMENT_DATA FragmentTable[1];
} EFI_IP6_RECEIVE_DATA;

typedef struct {
    EFI_EVENT                 Event;
    EFI_STATUS                Status;
    union {
	EFI_IP6_RECEIVE_DATA  *RxData;
	EFI_IP6_TRANSMIT_DATA *TxData;
    }                         Packet;
} EFI_IP6_COMPLETION_TOKEN;

typedef
EFI_STATUS
(EFIAPI *EFI_IP6_TRANSMIT) (
    IN struct _EFI_IP6          *This,
    IN EFI_IP6_COMPLETION_TOKEN *Token
    );

typedef
EFI_STATUS
(EFIAPI *EFI_IP6_RECEIVE) (
    IN struct _EFI_IP6          *This,
    IN EFI_IP6_COMPLETION_TOKEN *Token
    );

typedef
EFI_STATUS
(EFIAPI *EFI_IP6_CANCEL)(
    IN struct _EFI_IP6          *This,
    IN EFI_IP6_COMPLETION_TOKEN *Token OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_IP6_POLL) (
    IN struct _EFI_IP6 *This
    );

typedef struct _EFI_IP6 {
    EFI_IP6_GET_MODE_DATA GetModeData;
    EFI_IP6_CONFIGURE     Configure;
    EFI_IP6_GROUPS        Groups;
    EFI_IP6_ROUTES        Routes;
    EFI_IP6_NEIGHBORS     Neighbors;
    EFI_IP6_TRANSMIT      Transmit;
    EFI_IP6_RECEIVE       Receive;
    EFI_IP6_CANCEL        Cancel;
    EFI_IP6_POLL          Poll;
} EFI_IP6;

#endif /* _EFI_IP_H */
