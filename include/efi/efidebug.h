#ifndef _EFI_DEBUG_H
#define _EFI_DEBUG_H

/*++

Copyright (c) 1998  Intel Corporation

Module Name:

    efidebug.h

Abstract:

    EFI library debug functions



Revision History

--*/

extern uint64_t     EFIDebug;

#if EFI_DEBUG

    #define DBGASSERT(a)        DbgAssert(__FILE__, __LINE__, #a)
    #define DEBUG(a)            DbgPrint a
    
#else

    #define DBGASSERT(a)
    #define DEBUG(a)
    
#endif

#if EFI_DEBUG_CLEAR_MEMORY

    #define DBGSETMEM(a,l)      SetMem(a,l,(const char)BAD_POINTER)

#else

    #define DBGSETMEM(a,l)

#endif

#define D_INIT        0x00000001          // Initialization style messages
#define D_WARN        0x00000002          // Warnings
#define D_LOAD        0x00000004          // Load events
#define D_FS          0x00000008          // EFI File system
#define D_POOL        0x00000010          // Alloc & Free's
#define D_PAGE        0x00000020          // Alloc & Free's
#define D_INFO        0x00000040          // Verbose
#define D_VAR         0x00000100          // Variable
#define D_PARSE       0x00000200          // Command parsing
#define D_BM          0x00000400          // Boot manager
#define D_BLKIO       0x00001000          // BlkIo Driver
#define D_BLKIO_ULTRA 0x00002000          // BlkIo Driver
#define D_NET         0x00004000          // SNI Driver
#define D_NET_ULTRA   0x00008000          // SNI Driver
#define D_TXTIN       0x00010000          // Simple Input Driver
#define D_TXTOUT      0x00020000          // Simple Text Output Driver
#define D_ERROR_ATA	  0x00040000		  		// ATA error messages 
#define D_ERROR       0x80000000          // Error

#define D_RESERVED    0x7fffC880          // Bits not reserved above

//
// Current Debug level of the system, value of EFIDebug
//
//#define EFI_DBUG_MASK   (D_ERROR | D_WARN | D_LOAD | D_BLKIO | D_INIT)
#define EFI_DBUG_MASK   (D_ERROR)

//
//
//

#if EFI_DEBUG

    #define ASSERT(a)               if(!(a))       DBGASSERT(a)
    #define ASSERT_LOCKED(l)        if(!(l)->Lock) DBGASSERT(l not locked)
    #define ASSERT_STRUCT(p,t)      DBGASSERT(t not structure), p

#else

    #define ASSERT(a)               
    #define ASSERT_LOCKED(l)        
    #define ASSERT_STRUCT(p,t)      

#endif

//
// Prototypes
//

int64_t
DbgAssert (
    const const char   *file,
    int64_t          lineno,
    const const char   *string
    );

int64_t
DbgPrint (
    int64_t          mask,
    const const char   *format,
    ...
    );

//
// Instruction Set Architectures definitions for debuggers
//

typedef int64_t EFI_EXCEPTION_TYPE;

// IA32
#define EXCEPT_IA32_DIVIDE_ERROR    0
#define EXCEPT_IA32_DEBUG           1
#define EXCEPT_IA32_NMI             2
#define EXCEPT_IA32_BREAKPOINT      3
#define EXCEPT_IA32_OVERFLOW        4
#define EXCEPT_IA32_BOUND           5
#define EXCEPT_IA32_INVALID_OPCODE  6
#define EXCEPT_IA32_DOUBLE_FAULT    8
#define EXCEPT_IA32_INVALID_TSS     10
#define EXCEPT_IA32_SEG_NOT_PRESENT 11
#define EXCEPT_IA32_STACK_FAULT     12
#define EXCEPT_IA32_GP_FAULT        13
#define EXCEPT_IA32_PAGE_FAULT      14
#define EXCEPT_IA32_FP_ERROR        16
#define EXCEPT_IA32_ALIGNMENT_CHECK 17
#define EXCEPT_IA32_MACHINE_CHECK   18
#define EXCEPT_IA32_SIMD            19

typedef struct {
    uint16_t  Fcw;
    uint16_t  Fsw;
    uint16_t  Ftw;
    uint16_t  Opcode;
    uint32_t  Eip;
    uint16_t  Cs;
    uint16_t  Reserved1;
    uint32_t  DataOffset;
    uint16_t  Ds;
    uint8_t   Reserved2[10];
    uint8_t   St0Mm0[10], Reserved3[6];
    uint8_t   St1Mm1[10], Reserved4[6];
    uint8_t   St2Mm2[10], Reserved5[6];
    uint8_t   St3Mm3[10], Reserved6[6];
    uint8_t   St4Mm4[10], Reserved7[6];
    uint8_t   St5Mm5[10], Reserved8[6];
    uint8_t   St6Mm6[10], Reserved9[6];
    uint8_t   St7Mm7[10], Reserved10[6];
    uint8_t   Xmm0[16];
    uint8_t   Xmm1[16];
    uint8_t   Xmm2[16];
    uint8_t   Xmm3[16];
    uint8_t   Xmm4[16];
    uint8_t   Xmm5[16];
    uint8_t   Xmm6[16];
    uint8_t   Xmm7[16];
    uint8_t   Reserved11[14 * 16];
} EFI_FX_SAVE_STATE_IA32;

typedef struct {
    uint32_t                 ExceptionData;
    EFI_FX_SAVE_STATE_IA32 FxSaveState;
    uint32_t                 Dr0;
    uint32_t                 Dr1;
    uint32_t                 Dr2;
    uint32_t                 Dr3;
    uint32_t                 Dr6;
    uint32_t                 Dr7;
    uint32_t                 Cr0;
    uint32_t                 Cr1;
    uint32_t                 Cr2;
    uint32_t                 Cr3;
    uint32_t                 Cr4;
    uint32_t                 Eflags;
    uint32_t                 Ldtr;
    uint32_t                 Tr;
    uint32_t                 Gdtr[2];
    uint32_t                 Idtr[2];
    uint32_t                 Eip;
    uint32_t                 Gs;
    uint32_t                 Fs;
    uint32_t                 Es;
    uint32_t                 Ds;
    uint32_t                 Cs;
    uint32_t                 Ss;
    uint32_t                 Edi;
    uint32_t                 Esi;
    uint32_t                 Ebp;
    uint32_t                 Esp;
    uint32_t                 Ebx;
    uint32_t                 Edx;
    uint32_t                 Ecx;
    uint32_t                 Eax;
} EFI_SYSTEM_CONTEXT_IA32;

// X64
#define EXCEPT_X64_DIVIDE_ERROR    0
#define EXCEPT_X64_DEBUG           1
#define EXCEPT_X64_NMI             2
#define EXCEPT_X64_BREAKPOINT      3
#define EXCEPT_X64_OVERFLOW        4
#define EXCEPT_X64_BOUND           5
#define EXCEPT_X64_INVALID_OPCODE  6
#define EXCEPT_X64_DOUBLE_FAULT    8
#define EXCEPT_X64_INVALID_TSS     10
#define EXCEPT_X64_SEG_NOT_PRESENT 11
#define EXCEPT_X64_STACK_FAULT     12
#define EXCEPT_X64_GP_FAULT        13
#define EXCEPT_X64_PAGE_FAULT      14
#define EXCEPT_X64_FP_ERROR        16
#define EXCEPT_X64_ALIGNMENT_CHECK 17
#define EXCEPT_X64_MACHINE_CHECK   18
#define EXCEPT_X64_SIMD            19

typedef struct {
    uint16_t  Fcw;
    uint16_t  Fsw;
    uint16_t  Ftw;
    uint16_t  Opcode;
    uint64_t  Rip;
    uint64_t  DataOffset;
    uint8_t   Reserved1[8];
    uint8_t   St0Mm0[10], Reserved2[6];
    uint8_t   St1Mm1[10], Reserved3[6];
    uint8_t   St2Mm2[10], Reserved4[6];
    uint8_t   St3Mm3[10], Reserved5[6];
    uint8_t   St4Mm4[10], Reserved6[6];
    uint8_t   St5Mm5[10], Reserved7[6];
    uint8_t   St6Mm6[10], Reserved8[6];
    uint8_t   St7Mm7[10], Reserved9[6];
    uint8_t   Xmm0[16];
    uint8_t   Xmm1[16];
    uint8_t   Xmm2[16];
    uint8_t   Xmm3[16];
    uint8_t   Xmm4[16];
    uint8_t   Xmm5[16];
    uint8_t   Xmm6[16];
    uint8_t   Xmm7[16];
    uint8_t   Reserved11[14 * 16];
} EFI_FX_SAVE_STATE_X64;

typedef struct {
    uint64_t                ExceptionData;
    EFI_FX_SAVE_STATE_X64 FxSaveState;
    uint64_t                Dr0;
    uint64_t                Dr1;
    uint64_t                Dr2;
    uint64_t                Dr3;
    uint64_t                Dr6;
    uint64_t                Dr7;
    uint64_t                Cr0;
    uint64_t                Cr1;
    uint64_t                Cr2;
    uint64_t                Cr3;
    uint64_t                Cr4;
    uint64_t                Cr8;
    uint64_t                Rflags;
    uint64_t                Ldtr;
    uint64_t                Tr;
    uint64_t                Gdtr[2];
    uint64_t                Idtr[2];
    uint64_t                Rip;
    uint64_t                Gs;
    uint64_t                Fs;
    uint64_t                Es;
    uint64_t                Ds;
    uint64_t                Cs;
    uint64_t                Ss;
    uint64_t                Rdi;
    uint64_t                Rsi;
    uint64_t                Rbp;
    uint64_t                Rsp;
    uint64_t                Rbx;
    uint64_t                Rdx;
    uint64_t                Rcx;
    uint64_t                Rax;
    uint64_t                R8;
    uint64_t                R9;
    uint64_t                R10;
    uint64_t                R11;
    uint64_t                R12;
    uint64_t                R13;
    uint64_t                R14;
    uint64_t                R15;
} EFI_SYSTEM_CONTEXT_X64;

/// IA64
#define EXCEPT_IPF_VHTP_TRANSLATION               0
#define EXCEPT_IPF_INSTRUCTION_TLB                1
#define EXCEPT_IPF_DATA_TLB                       2
#define EXCEPT_IPF_ALT_INSTRUCTION_TLB            3
#define EXCEPT_IPF_ALT_DATA_TLB                   4
#define EXCEPT_IPF_DATA_NESTED_TLB                5
#define EXCEPT_IPF_INSTRUCTION_KEY_MISSED         6
#define EXCEPT_IPF_DATA_KEY_MISSED                7
#define EXCEPT_IPF_DIRTY_BIT                      8
#define EXCEPT_IPF_INSTRUCTION_ACCESS_BIT         9
#define EXCEPT_IPF_DATA_ACCESS_BIT                10
#define EXCEPT_IPF_BREAKPOINT                     11
#define EXCEPT_IPF_EXTERNAL_INTERRUPT             12
#define EXCEPT_IPF_PAGE_NOT_PRESENT               20
#define EXCEPT_IPF_KEY_PERMISSION                 21
#define EXCEPT_IPF_INSTRUCTION_ACCESS_RIGHTS      22
#define EXCEPT_IPF_DATA_ACCESS_RIGHTS             23
#define EXCEPT_IPF_GENERAL_EXCEPTION              24
#define EXCEPT_IPF_DISABLED_FP_REGISTER           25
#define EXCEPT_IPF_NAT_CONSUMPTION                26
#define EXCEPT_IPF_SPECULATION                    27
#define EXCEPT_IPF_DEBUG                          29
#define EXCEPT_IPF_UNALIGNED_REFERENCE            30
#define EXCEPT_IPF_UNSUPPORTED_DATA_REFERENCE     31
#define EXCEPT_IPF_FP_FAULT                       32
#define EXCEPT_IPF_FP_TRAP                        33
#define EXCEPT_IPF_LOWER_PRIVILEGE_TRANSFER_TRAP  34
#define EXCEPT_IPF_TAKEN_BRANCH                   35
#define EXCEPT_IPF_SINGLE_STEP                    36
#define EXCEPT_IPF_IA32_EXCEPTION                 45
#define EXCEPT_IPF_IA32_INTERCEPT                 46
#define EXCEPT_IPF_IA32_INTERRUPT                 47

typedef struct {
    uint64_t  Reserved;
    uint64_t  R1;
    uint64_t  R2;
    uint64_t  R3;
    uint64_t  R4;
    uint64_t  R5;
    uint64_t  R6;
    uint64_t  R7;
    uint64_t  R8;
    uint64_t  R9;
    uint64_t  R10;
    uint64_t  R11;
    uint64_t  R12;
    uint64_t  R13;
    uint64_t  R14;
    uint64_t  R15;
    uint64_t  R16;
    uint64_t  R17;
    uint64_t  R18;
    uint64_t  R19;
    uint64_t  R20;
    uint64_t  R21;
    uint64_t  R22;
    uint64_t  R23;
    uint64_t  R24;
    uint64_t  R25;
    uint64_t  R26;
    uint64_t  R27;
    uint64_t  R28;
    uint64_t  R29;
    uint64_t  R30;
    uint64_t  R31;
    uint64_t  F2[2];
    uint64_t  F3[2];
    uint64_t  F4[2];
    uint64_t  F5[2];
    uint64_t  F6[2];
    uint64_t  F7[2];
    uint64_t  F8[2];
    uint64_t  F9[2];
    uint64_t  F10[2];
    uint64_t  F11[2];
    uint64_t  F12[2];
    uint64_t  F13[2];
    uint64_t  F14[2];
    uint64_t  F15[2];
    uint64_t  F16[2];
    uint64_t  F17[2];
    uint64_t  F18[2];
    uint64_t  F19[2];
    uint64_t  F20[2];
    uint64_t  F21[2];
    uint64_t  F22[2];
    uint64_t  F23[2];
    uint64_t  F24[2];
    uint64_t  F25[2];
    uint64_t  F26[2];
    uint64_t  F27[2];
    uint64_t  F28[2];
    uint64_t  F29[2];
    uint64_t  F30[2];
    uint64_t  F31[2];
    uint64_t  Pr;
    uint64_t  B0;
    uint64_t  B1;
    uint64_t  B2;
    uint64_t  B3;
    uint64_t  B4;
    uint64_t  B5;
    uint64_t  B6;
    uint64_t  B7;
    uint64_t  ArRsc;
    uint64_t  ArBsp;
    uint64_t  ArBspstore;
    uint64_t  ArRnat;
    uint64_t  ArFcr;
    uint64_t  ArEflag;
    uint64_t  ArCsd;
    uint64_t  ArSsd;
    uint64_t  ArCflg;
    uint64_t  ArFsr;
    uint64_t  ArFir;
    uint64_t  ArFdr;
    uint64_t  ArCcv;
    uint64_t  ArUnat;
    uint64_t  ArFpsr;
    uint64_t  ArPfs;
    uint64_t  ArLc;
    uint64_t  ArEc;
    uint64_t  CrDcr;
    uint64_t  CrItm;
    uint64_t  CrIva;
    uint64_t  CrPta;
    uint64_t  CrIpsr;
    uint64_t  CrIsr;
    uint64_t  CrIip;
    uint64_t  CrIfa;
    uint64_t  CrItir;
    uint64_t  CrIipa;
    uint64_t  CrIfs;
    uint64_t  CrIim;
    uint64_t  CrIha;
    uint64_t  Dbr0;
    uint64_t  Dbr1;
    uint64_t  Dbr2;
    uint64_t  Dbr3;
    uint64_t  Dbr4;
    uint64_t  Dbr5;
    uint64_t  Dbr6;
    uint64_t  Dbr7;
    uint64_t  Ibr0;
    uint64_t  Ibr1;
    uint64_t  Ibr2;
    uint64_t  Ibr3;
    uint64_t  Ibr4;
    uint64_t  Ibr5;
    uint64_t  Ibr6;
    uint64_t  Ibr7;
    uint64_t  IntNat;
} EFI_SYSTEM_CONTEXT_IPF;

// EBC
#define EXCEPT_EBC_UNDEFINED            0
#define EXCEPT_EBC_DIVIDE_ERROR         1
#define EXCEPT_EBC_DEBUG                2
#define EXCEPT_EBC_BREAKPOINT           3
#define EXCEPT_EBC_OVERFLOW             4
#define EXCEPT_EBC_INVALID_OPCODE       5
#define EXCEPT_EBC_STACK_FAULT          6
#define EXCEPT_EBC_ALIGNMENT_CHECK      7
#define EXCEPT_EBC_INSTRUCTION_ENCODING 8
#define EXCEPT_EBC_BAD_BREAK            9
#define EXCEPT_EBC_STEP                 10
#define MAX_EBC_EXCEPTION               EXCEPT_EBC_STEP

typedef struct {
    uint64_t  R0;
    uint64_t  R1;
    uint64_t  R2;
    uint64_t  R3;
    uint64_t  R4;
    uint64_t  R5;
    uint64_t  R6;
    uint64_t  R7;
    uint64_t  Flags;
    uint64_t  ControlFlags;
    uint64_t  Ip;
} EFI_SYSTEM_CONTEXT_EBC;

// ARM
#define EXCEPT_ARM_RESET                    0
#define EXCEPT_ARM_UNDEFINED_INSTRUCTION    1
#define EXCEPT_ARM_SOFTWARE_INTERRUPT       2
#define EXCEPT_ARM_PREFETCH_ABORT           3
#define EXCEPT_ARM_DATA_ABORT               4
#define EXCEPT_ARM_RESERVED                 5
#define EXCEPT_ARM_IRQ                      6
#define EXCEPT_ARM_FIQ                      7
#define MAX_ARM_EXCEPTION                   EXCEPT_ARM_FIQ

typedef struct {
    uint32_t  R0;
    uint32_t  R1;
    uint32_t  R2;
    uint32_t  R3;
    uint32_t  R4;
    uint32_t  R5;
    uint32_t  R6;
    uint32_t  R7;
    uint32_t  R8;
    uint32_t  R9;
    uint32_t  R10;
    uint32_t  R11;
    uint32_t  R12;
    uint32_t  SP;
    uint32_t  LR;
    uint32_t  PC;
    uint32_t  CPSR;
    uint32_t  DFSR;
    uint32_t  DFAR;
    uint32_t  IFSR;
    uint32_t  IFAR;
} EFI_SYSTEM_CONTEXT_ARM;


typedef union {
    EFI_SYSTEM_CONTEXT_EBC  *SystemContextEbc;
    EFI_SYSTEM_CONTEXT_IA32 *SystemContextIa32;
    EFI_SYSTEM_CONTEXT_X64  *SystemContextX64;
    EFI_SYSTEM_CONTEXT_IPF  *SystemContextIpf;
    EFI_SYSTEM_CONTEXT_ARM  *SystemContextArm;
} EFI_SYSTEM_CONTEXT;

typedef
void
(EFIAPI *EFI_EXCEPTION_CALLBACK)(
    IN     EFI_EXCEPTION_TYPE               ExceptionType,
    IN OUT EFI_SYSTEM_CONTEXT               SystemContext);

typedef
void
(EFIAPI *EFI_PERIODIC_CALLBACK)(
    IN OUT EFI_SYSTEM_CONTEXT               SystemContext);

typedef enum {
    IsaIa32 = EFI_IMAGE_MACHINE_IA32,
    IsaX64 = EFI_IMAGE_MACHINE_X64,
    IsaIpf = EFI_IMAGE_MACHINE_IA64,
    IsaEbc = EFI_IMAGE_MACHINE_EBC,
    IsaArm = EFI_IMAGE_MACHINE_ARMTHUMB_MIXED,
//	IsaArm64 = EFI_IMAGE_MACHINE_AARCH64
} EFI_INSTRUCTION_SET_ARCHITECTURE;

//
// DEBUG_IMAGE_INFO
//

#define EFI_DEBUG_IMAGE_INFO_TABLE_GUID \
    { 0x49152e77, 0x1ada, 0x4764, {0xb7, 0xa2, 0x7a, 0xfe, 0xfe, 0xd9, 0x5e, 0x8b} }

#define EFI_DEBUG_IMAGE_INFO_UPDATE_IN_PROGRESS 0x01
#define EFI_DEBUG_IMAGE_INFO_TABLE_MODIFIED     0x02
#define EFI_DEBUG_IMAGE_INFO_INITIAL_SIZE       (EFI_PAGE_SIZE / sizeof (uint64_t))
#define EFI_DEBUG_IMAGE_INFO_TYPE_NORMAL        0x01

typedef struct {
uint64_t                Signature;
EFI_PHYSICAL_ADDRESS  EfiSystemTableBase;
uint32_t                Crc32;
} EFI_SYSTEM_TABLE_POINTER;

typedef struct {
uint32_t                    ImageInfoType;
EFI_LOADED_IMAGE_PROTOCOL *LoadedImageProtocolInstance;
EFI_HANDLE                *ImageHandle;
} EFI_DEBUG_IMAGE_INFO_NORMAL;

typedef union {
uint32_t                      *ImageInfoType;
EFI_DEBUG_IMAGE_INFO_NORMAL *NormalImage;
} EFI_DEBUG_IMAGE_INFO;

typedef struct {
volatile uint32_t       UpdateStatus;
uint32_t                TableSize;
EFI_DEBUG_IMAGE_INFO  *EfiDebugImageInfoTable;
} EFI_DEBUG_IMAGE_INFO_TABLE_HEADER;

//
// EFI_DEBUGGER_PROTOCOL
//

#define EFI_DEBUG_SUPPORT_PROTOCOL_GUID \
    { 0x2755590c, 0x6f3c, 0x42fa, {0x9e, 0xa4, 0xa3, 0xba, 0x54, 0x3c, 0xda, 0x25} }

INTERFACE_DECL(_EFI_DEBUG_SUPPORT_PROTOCOL);

typedef
EFI_STATUS
(EFIAPI *EFI_GET_MAXIMUM_PROCESSOR_INDEX)(
    IN struct _EFI_DEBUG_SUPPORT_PROTOCOL  *This,
    OUT uint64_t                              *MaxProcessorIndex);

typedef
EFI_STATUS
(EFIAPI *EFI_REGISTER_PERIODIC_CALLBACK)(
    IN struct _EFI_DEBUG_SUPPORT_PROTOCOL  *This,
    IN uint64_t                               ProcessorIndex,
    IN EFI_PERIODIC_CALLBACK               PeriodicCallback);

typedef
EFI_STATUS
(EFIAPI *EFI_REGISTER_EXCEPTION_CALLBACK)(
    IN struct _EFI_DEBUG_SUPPORT_PROTOCOL  *This,
    IN uint64_t                               ProcessorIndex,
    IN EFI_EXCEPTION_CALLBACK              ExceptionCallback,
    IN EFI_EXCEPTION_TYPE                  ExceptionType);

typedef
EFI_STATUS
(EFIAPI *EFI_INVALIDATE_INSTRUCTION_CACHE)(
    IN struct _EFI_DEBUG_SUPPORT_PROTOCOL  *This,
    IN uint64_t                               ProcessorIndex,
    IN void                                *Start,
    IN uint64_t                              Length);

typedef struct _EFI_DEBUG_SUPPORT_PROTOCOL {
    EFI_INSTRUCTION_SET_ARCHITECTURE  Isa;
    EFI_GET_MAXIMUM_PROCESSOR_INDEX   GetMaximumProcessorIndex;
    EFI_REGISTER_PERIODIC_CALLBACK    RegisterPeriodicCallback;
    EFI_REGISTER_EXCEPTION_CALLBACK   RegisterExceptionCallback;
    EFI_INVALIDATE_INSTRUCTION_CACHE  InvalidateInstructionCache;
} EFI_DEBUG_SUPPORT_PROTOCOL;

#endif
