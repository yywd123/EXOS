#ifndef _EFI_TCP_H
#define _EFI_TCP_H

/*++
Copyright (c) 2013  Intel Corporation

--*/

#define EFI_TCP4_SERVICE_BINDING_PROTOCOL \
    { 0x00720665, 0x67eb, 0x4a99, {0xba, 0xf7, 0xd3, 0xc3, 0x3a, 0x1c,0x7c, 0xc9}}

#define EFI_TCP4_PROTOCOL \
    { 0x65530bc7, 0xa359, 0x410f, {0xb0, 0x10, 0x5a, 0xad, 0xc7, 0xec, 0x2b, 0x62}}

#define EFI_TCP6_SERVICE_BINDING_PROTOCOL \
    { 0xec20eb79, 0x6c1a, 0x4664, {0x9a, 0xd, 0xd2, 0xe4, 0xcc, 0x16, 0xd6, 0x64}}

#define EFI_TCP6_PROTOCOL \
    { 0x46e44855, 0xbd60, 0x4ab7, {0xab, 0xd, 0xa6, 0x79, 0xb9, 0x44, 0x7d, 0x77}}

INTERFACE_DECL(_EFI_TCP4);
INTERFACE_DECL(_EFI_TCP6);

typedef struct {
    bool            UseDefaultAddress;
    EFI_IPv4_ADDRESS   StationAddress;
    EFI_IPv4_ADDRESS   SubnetMask;
    uint16_t             StationPort;
    EFI_IPv4_ADDRESS   RemoteAddress;
    uint16_t             RemotePort;
    bool            ActiveFlag;
} EFI_TCP4_ACCESS_POINT;

typedef struct {
    uint32_t             ReceiveBufferSize;
    uint32_t             SendBufferSize;
    uint32_t             MaxSynBackLog;
    uint32_t             ConnectionTimeout;
    uint32_t             DataRetries;
    uint32_t             FinTimeout;
    uint32_t             TimeWaitTimeout;
    uint32_t             KeepAliveProbes;
    uint32_t             KeepAliveTime;
    uint32_t             KeepAliveInterval;
    bool            EnableNagle;
    bool            EnableTimeStamp;
    bool            EnableWindowScaling;
    bool            EnableSelectiveAck;
    bool            EnablePAthMtuDiscovery;
} EFI_TCP4_OPTION;

typedef struct {
    // Receiving Filters
    // I/O parameters
    uint8_t                 TypeOfService;
    uint8_t                 TimeToLive;

    // Access Point
    EFI_TCP4_ACCESS_POINT AccessPoint;

    // TCP Control Options
    EFI_TCP4_OPTION       *ControlOption;
} EFI_TCP4_CONFIG_DATA;

typedef enum {
    Tcp4StateClosed      = 0,
    Tcp4StateListen      = 1,
    Tcp4StateSynSent     = 2,
    Tcp4StateSynReceived = 3,
    Tcp4StateEstablished = 4,
    Tcp4StateFinWait1    = 5,
    Tcp4StateFinWait2    = 6,
    Tcp4StateClosing     = 7,
    Tcp4StateTimeWait    = 8,
    Tcp4StateCloseWait   = 9,
    Tcp4StateLastAck     = 10
} EFI_TCP4_CONNECTION_STATE;

typedef
EFI_STATUS
(EFIAPI *EFI_TCP4_GET_MODE_DATA) (
    IN struct _EFI_TCP4                 *This,
    OUT EFI_TCP4_CONNECTION_STATE       *Tcp4State      OPTIONAL,
    OUT EFI_TCP4_CONFIG_DATA            *Tcp4ConfigData OPTIONAL,
    OUT EFI_IP4_MODE_DATA               *Ip4ModeData    OPTIONAL,
    OUT EFI_MANAGED_NETWORK_CONFIG_DATA *MnpConfigData  OPTIONAL,
    OUT EFI_SIMPLE_NETWORK_MODE         *SnpModeData    OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_TCP4_CONFIGURE) (
    IN struct _EFI_TCP4     *This,
    IN EFI_TCP4_CONFIG_DATA *TcpConfigData OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_TCP4_ROUTES) (
    IN struct _EFI_TCP4 *This,
    IN bool          DeleteRoute,
    IN EFI_IPv4_ADDRESS *SubnetAddress,
    IN EFI_IPv4_ADDRESS *SubnetMask,
    IN EFI_IPv4_ADDRESS *GatewayAddress
);

typedef struct {
    EFI_EVENT  Event;
    EFI_STATUS Status;
} EFI_TCP4_COMPLETION_TOKEN;

typedef struct {
    EFI_TCP4_COMPLETION_TOKEN CompletionToken;
} EFI_TCP4_CONNECTION_TOKEN;

typedef
EFI_STATUS
(EFIAPI *EFI_TCP4_CONNECT) (
    IN struct _EFI_TCP4          *This,
    IN EFI_TCP4_CONNECTION_TOKEN *ConnectionToken
    );

typedef struct {
    EFI_TCP4_COMPLETION_TOKEN CompletionToken;
    EFI_HANDLE                NewChildHandle;
} EFI_TCP4_LISTEN_TOKEN;

typedef
EFI_STATUS
(EFIAPI *EFI_TCP4_ACCEPT) (
    IN struct _EFI_TCP4      *This,
    IN EFI_TCP4_LISTEN_TOKEN *ListenToken
    );

#define EFI_CONNECTION_FIN     EFIERR(104)
#define EFI_CONNECTION_RESET   EFIERR(105)
#define EFI_CONNECTION_REFUSED EFIERR(106)

typedef struct {
    uint32_t FragmentLength;
    void   *FragmentBuffer;
} EFI_TCP4_FRAGMENT_DATA;

typedef struct {
    bool                UrgentFlag;
    uint32_t                 DataLength;
    uint32_t                 FragmentCount;
    EFI_TCP4_FRAGMENT_DATA FragmentTable[1];
} EFI_TCP4_RECEIVE_DATA;

typedef struct {
    bool                Push;
    bool                Urgent;
    uint32_t                 DataLength;
    uint32_t                 FragmentCount;
    EFI_TCP4_FRAGMENT_DATA FragmentTable[1];
} EFI_TCP4_TRANSMIT_DATA;

typedef struct {
    EFI_TCP4_COMPLETION_TOKEN  CompletionToken;
    union {
	EFI_TCP4_RECEIVE_DATA  *RxData;
	EFI_TCP4_TRANSMIT_DATA *TxData;
    }                          Packet;
} EFI_TCP4_IO_TOKEN;

typedef
EFI_STATUS
(EFIAPI *EFI_TCP4_TRANSMIT) (
    IN struct _EFI_TCP4  *This,
    IN EFI_TCP4_IO_TOKEN *Token
    );

typedef
EFI_STATUS
(EFIAPI *EFI_TCP4_RECEIVE) (
    IN struct _EFI_TCP4  *This,
    IN EFI_TCP4_IO_TOKEN *Token
    );

typedef struct {
    EFI_TCP4_COMPLETION_TOKEN CompletionToken;
    bool                   AbortOnClose;
} EFI_TCP4_CLOSE_TOKEN;

typedef
EFI_STATUS
(EFIAPI *EFI_TCP4_CLOSE)(
    IN struct _EFI_TCP4     *This,
    IN EFI_TCP4_CLOSE_TOKEN *CloseToken
    );

typedef
EFI_STATUS
(EFIAPI *EFI_TCP4_CANCEL)(
    IN struct _EFI_TCP4 *This,
    IN EFI_TCP4_COMPLETION_TOKEN *Token OPTIONAL
);

typedef
EFI_STATUS
(EFIAPI *EFI_TCP4_POLL) (
    IN struct _EFI_TCP4 *This
    );

typedef struct _EFI_TCP4 {
    EFI_TCP4_GET_MODE_DATA GetModeData;
    EFI_TCP4_CONFIGURE     Configure;
    EFI_TCP4_ROUTES        Routes;
    EFI_TCP4_CONNECT       Connect;
    EFI_TCP4_ACCEPT        Accept;
    EFI_TCP4_TRANSMIT      Transmit;
    EFI_TCP4_RECEIVE       Receive;
    EFI_TCP4_CLOSE         Close;
    EFI_TCP4_CANCEL        Cancel;
    EFI_TCP4_POLL          Poll;
} EFI_TCP4;

typedef enum {
    Tcp6StateClosed      = 0,
    Tcp6StateListen      = 1,
    Tcp6StateSynSent     = 2,
    Tcp6StateSynReceived = 3,
    Tcp6StateEstablished = 4,
    Tcp6StateFinWait1    = 5,
    Tcp6StateFinWait2    = 6,
    Tcp6StateClosing     = 7,
    Tcp6StateTimeWait    = 8,
    Tcp6StateCloseWait   = 9,
    Tcp6StateLastAck     = 10
} EFI_TCP6_CONNECTION_STATE;

typedef struct {
    EFI_IPv6_ADDRESS StationAddress;
    uint16_t           StationPort;
    EFI_IPv6_ADDRESS RemoteAddress;
    uint16_t           RemotePort;
    bool          ActiveFlag;
} EFI_TCP6_ACCESS_POINT;

typedef struct {
    uint32_t             ReceiveBufferSize;
    uint32_t             SendBufferSize;
    uint32_t             MaxSynBackLog;
    uint32_t             ConnectionTimeout;
    uint32_t             DataRetries;
    uint32_t             FinTimeout;
    uint32_t             TimeWaitTimeout;
    uint32_t             KeepAliveProbes;
    uint32_t             KeepAliveTime;
    uint32_t             KeepAliveInterval;
    bool            EnableNagle;
    bool            EnableTimeStamp;
    bool            EnableWindbowScaling;
    bool            EnableSelectiveAck;
    bool            EnablePathMtuDiscovery;
} EFI_TCP6_OPTION;

typedef struct {
    uint8_t                 TrafficClass;
    uint8_t                 HopLimit;
    EFI_TCP6_ACCESS_POINT AccessPoint;
    EFI_TCP6_OPTION       *ControlOption;
} EFI_TCP6_CONFIG_DATA;

typedef
EFI_STATUS
(EFIAPI *EFI_TCP6_GET_MODE_DATA) (
    IN struct _EFI_TCP6                 *This,
    OUT EFI_TCP6_CONNECTION_STATE       *Tcp6State      OPTIONAL,
    OUT EFI_TCP6_CONFIG_DATA            *Tcp6ConfigData OPTIONAL,
    OUT EFI_IP6_MODE_DATA               *Ip6ModeData    OPTIONAL,
    OUT EFI_MANAGED_NETWORK_CONFIG_DATA *MnpConfigData  OPTIONAL,
    OUT EFI_SIMPLE_NETWORK_MODE         *SnpModeData    OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_TCP6_CONFIGURE) (
    IN struct _EFI_TCP6     *This,
    IN EFI_TCP6_CONFIG_DATA *Tcp6ConfigData OPTIONAL
    );

typedef struct {
    EFI_EVENT  Event;
    EFI_STATUS Status;
} EFI_TCP6_COMPLETION_TOKEN;

typedef struct {
    EFI_TCP6_COMPLETION_TOKEN CompletionToken;
} EFI_TCP6_CONNECTION_TOKEN;

typedef
EFI_STATUS
(EFIAPI *EFI_TCP6_CONNECT) (
    IN struct _EFI_TCP6          *This,
    IN EFI_TCP6_CONNECTION_TOKEN *ConnectionToken
    );

typedef struct {
    EFI_TCP6_COMPLETION_TOKEN CompletionToken;
    EFI_HANDLE                NewChildHandle;
} EFI_TCP6_LISTEN_TOKEN;

typedef
EFI_STATUS
(EFIAPI *EFI_TCP6_ACCEPT) (
    IN struct _EFI_TCP6      *This,
    IN EFI_TCP6_LISTEN_TOKEN *ListenToken
    );

typedef struct {
    uint32_t FragmentLength;
    void   *FragmentBuffer;
} EFI_TCP6_FRAGMENT_DATA;

typedef struct {
    bool                UrgentFlag;
    uint32_t                 DataLength;
    uint32_t                 FragmentCount;
    EFI_TCP6_FRAGMENT_DATA FragmentTable[1];
} EFI_TCP6_RECEIVE_DATA;

typedef struct {
    bool                Push;
    bool                Urgent;
    uint32_t                 DataLength;
    uint32_t                 FragmentCount;
    EFI_TCP6_FRAGMENT_DATA FragmentTable[1];
} EFI_TCP6_TRANSMIT_DATA;

typedef struct {
    EFI_TCP6_COMPLETION_TOKEN  CompletionToken;
    union {
	EFI_TCP6_RECEIVE_DATA  *RxData;
	EFI_TCP6_TRANSMIT_DATA *TxData;
    }                          Packet;
} EFI_TCP6_IO_TOKEN;

typedef
EFI_STATUS
(EFIAPI *EFI_TCP6_TRANSMIT) (
    IN struct _EFI_TCP6  *This,
    IN EFI_TCP6_IO_TOKEN *Token
    );

typedef
EFI_STATUS
(EFIAPI *EFI_TCP6_RECEIVE) (
    IN struct _EFI_TCP6  *This,
    IN EFI_TCP6_IO_TOKEN *Token
    );

typedef struct {
    EFI_TCP6_COMPLETION_TOKEN CompletionToken;
    bool                   AbortOnClose;
} EFI_TCP6_CLOSE_TOKEN;

typedef
EFI_STATUS
(EFIAPI *EFI_TCP6_CLOSE)(
    IN struct _EFI_TCP6     *This,
    IN EFI_TCP6_CLOSE_TOKEN *CloseToken
    );

typedef
EFI_STATUS
(EFIAPI *EFI_TCP6_CANCEL)(
    IN struct _EFI_TCP6          *This,
    IN EFI_TCP6_COMPLETION_TOKEN *Token OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_TCP6_POLL) (
    IN struct _EFI_TCP6 *This
    );

typedef struct _EFI_TCP6 {
    EFI_TCP6_GET_MODE_DATA GetModeData;
    EFI_TCP6_CONFIGURE     Configure;
    EFI_TCP6_CONNECT       Connect;
    EFI_TCP6_ACCEPT        Accept;
    EFI_TCP6_TRANSMIT      Transmit;
    EFI_TCP6_RECEIVE       Receive;
    EFI_TCP6_CLOSE         Close;
    EFI_TCP6_CANCEL        Cancel;
    EFI_TCP6_POLL          Poll;
} EFI_TCP6;

#endif /* _EFI_TCP_H */
