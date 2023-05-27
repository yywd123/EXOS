/*++

Copyright (c) 1998  Intel Corporation

Module Name:

    efefind.h

Abstract:

    EFI to compile bindings




Revision History

--*/
#ifndef X86_64_EFI_BIND
#define X86_64_EFI_BIND
#ifndef __GNUC__
#pragma pack()
#endif

#if defined(GNU_EFI_USE_MS_ABI)
    #if (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)))||(defined(__clang__) && (__clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 2)))
        #define HAVE_USE_MS_ABI 1
    #else
        #error Compiler is too old for GNU_EFI_USE_MS_ABI
    #endif
#endif

//
// Basic int types of various widths
//

#if !defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L )

    // No ANSI C 1999/2000 stdint.h integer width declarations 

    #if defined(_MSC_EXTENSIONS)

        // Use Microsoft C compiler integer width declarations 

        typedef unsigned __int64    uint64_t;
        typedef __int64             int64_t;
        typedef unsigned __int32    uint32_t;
        typedef __int32             int32_t;
        typedef unsigned short      uint16_t;
        typedef short               int16_t;
        typedef unsigned char       uint8_t;
        typedef char                int8_t;
    #elif defined(__GNUC__)
        typedef int __attribute__((__mode__(__DI__)))           int64_t;
        typedef unsigned int __attribute__((__mode__(__DI__)))  uint64_t;
        typedef unsigned int        uint32_t;
        typedef int                 int32_t;
        typedef unsigned short      uint16_t;
        typedef short               int16_t;
        typedef unsigned char       uint8_t;
        typedef signed char         int8_t;
    #elif defined(UNIX_LP64)

        /*  Use LP64 programming model from C_FLAGS for integer width declarations */

       typedef unsigned long       uint64_t;
       typedef long                int64_t;
       typedef unsigned int        uint32_t;
       typedef int                 int32_t;
       typedef unsigned short      uint16_t;
       typedef short               int16_t;
       typedef unsigned char       uint8_t;
       typedef char                int8_t;
    #else

       /*  Assume P64 programming model from C_FLAGS for integer width declarations */

       typedef unsigned long long  uint64_t __attribute__((aligned (8)));
       typedef long long           int64_t __attribute__((aligned (8)));
       typedef unsigned int        uint32_t;
       typedef int                 int32_t;
       typedef unsigned short      uint16_t;
       typedef short               int16_t;
       typedef unsigned char       uint8_t;
       typedef char                int8_t;
    #endif
#elif defined(__GNUC__)
    #include <stdint.h>
#endif

#define EFIERR(a)           (0x8000000000000000 | a)
#define EFI_ERROR_MASK      0x8000000000000000
#define EFIERR_OEM(a)       (0xc000000000000000 | a)      


#define BAD_POINTER         0xFBFBFBFBFBFBFBFB
#define MAX_ADDRESS         0xFFFFFFFFFFFFFFFF

#define BREAKPOINT()        while (true);

//
// Pointers must be aligned to these address to function
//

#define MIN_ALIGNMENT_SIZE  4

#define ALIGN_VARIABLE(Value ,Adjustment) \
            (UINTN)Adjustment = 0; \
            if((UINTN)Value % MIN_ALIGNMENT_SIZE) \
                (UINTN)Adjustment = MIN_ALIGNMENT_SIZE - ((UINTN)Value % MIN_ALIGNMENT_SIZE); \
            Value = (UINTN)Value + (UINTN)Adjustment


//
// Define macros to build data structure signatures from characters.
//

#define EFI_SIGNATURE_16(A,B)             ((A) | (B<<8))
#define EFI_SIGNATURE_32(A,B,C,D)         (EFI_SIGNATURE_16(A,B)     | (EFI_SIGNATURE_16(C,D)     << 16))
#define EFI_SIGNATURE_64(A,B,C,D,E,F,G,H) (EFI_SIGNATURE_32(A,B,C,D) | ((uint64_t)(EFI_SIGNATURE_32(E,F,G,H)) << 32))


//
// EFIAPI - prototype calling convention for EFI function pointers
// BOOTSERVICE - prototype for implementation of a boot service interface
// RUNTIMESERVICE - prototype for implementation of a runtime service interface
// RUNTIMEFUNCTION - prototype for implementation of a runtime function that is not a service
// RUNTIME_CODE - pragma macro for declaring runtime code    
//

#ifndef EFIAPI                  // Forces EFI calling conventions reguardless of compiler options 
    #ifdef _MSC_EXTENSIONS
        #define EFIAPI __cdecl  // Force C calling convention for Microsoft C compiler 
    #elif defined(HAVE_USE_MS_ABI)
        // Force amd64/ms calling conventions.
        #define EFIAPI __attribute__((ms_abi))
    #else
        #define EFIAPI          // Substitute expresion to force C calling convention 
    #endif
#endif

#define BOOTSERVICE
//#define RUNTIMESERVICE(proto,a)    alloc_text("rtcode",a); proto a
//#define RUNTIMEFUNCTION(proto,a)   alloc_text("rtcode",a); proto a
#define RUNTIMESERVICE
#define RUNTIMEFUNCTION


#define RUNTIME_CODE(a)         alloc_text("rtcode", a)
#define BEGIN_RUNTIME_DATA()    data_seg("rtdata")
#define END_RUNTIME_DATA()      data_seg("")

#define MEMORY_FENCE()    

//
// Some compilers don't support the forward reference construct:
//  typedef struct XXXXX
//
// The following macro provide a workaround for such cases.
//
#ifdef NO_INTERFACE_DECL
#define INTERFACE_DECL(x)
#else
#if defined(__GNUC__) || defined(_MSC_EXTENSIONS)
#define INTERFACE_DECL(x) struct x
#else
#define INTERFACE_DECL(x) typedef struct x
#endif
#endif

/* for x86_64, EFI_FUNCTION_WRAPPER must be defined */
#if defined(HAVE_USE_MS_ABI)
#define eficall(func, ...) func(__VA_ARGS__)
#else
/*
  Credits for macro-magic:
    https://groups.google.com/forum/?fromgroups#!topic/comp.std.c/d-6Mj5Lko_s
    http://efesx.com/2010/08/31/overloading-macros/
*/
#define __VA_NARG__(...)                        \
  __VA_NARG_(_0, ## __VA_ARGS__, __RSEQ_N())
#define __VA_NARG_(...)                         \
  __VA_ARG_N(__VA_ARGS__)
#define __VA_ARG_N(                             \
  _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,N,...) N
#define __RSEQ_N()                              \
  10, 9,  8,  7,  6,  5,  4,  3,  2,  1,  0

#define __VA_ARG_NSUFFIX__(prefix,...)                  \
  __VA_ARG_NSUFFIX_N(prefix, __VA_NARG__(__VA_ARGS__))
#define __VA_ARG_NSUFFIX_N(prefix,nargs)        \
  __VA_ARG_NSUFFIX_N_(prefix, nargs)
#define __VA_ARG_NSUFFIX_N_(prefix,nargs)       \
  prefix ## nargs

/* Prototypes of EFI cdecl -> stdcall trampolines */
uint64_t efi_call0(void *func);
uint64_t efi_call1(void *func, uint64_t arg1);
uint64_t efi_call2(void *func, uint64_t arg1, uint64_t arg2);
uint64_t efi_call3(void *func, uint64_t arg1, uint64_t arg2, uint64_t arg3);
uint64_t efi_call4(void *func, uint64_t arg1, uint64_t arg2, uint64_t arg3,
                 uint64_t arg4);
uint64_t efi_call5(void *func, uint64_t arg1, uint64_t arg2, uint64_t arg3,
                 uint64_t arg4, uint64_t arg5);
uint64_t efi_call6(void *func, uint64_t arg1, uint64_t arg2, uint64_t arg3,
                 uint64_t arg4, uint64_t arg5, uint64_t arg6);
uint64_t efi_call7(void *func, uint64_t arg1, uint64_t arg2, uint64_t arg3,
                 uint64_t arg4, uint64_t arg5, uint64_t arg6, uint64_t arg7);
uint64_t efi_call8(void *func, uint64_t arg1, uint64_t arg2, uint64_t arg3,
                 uint64_t arg4, uint64_t arg5, uint64_t arg6, uint64_t arg7,
                 uint64_t arg8);
uint64_t efi_call9(void *func, uint64_t arg1, uint64_t arg2, uint64_t arg3,
                 uint64_t arg4, uint64_t arg5, uint64_t arg6, uint64_t arg7,
                 uint64_t arg8, uint64_t arg9);
uint64_t efi_call10(void *func, uint64_t arg1, uint64_t arg2, uint64_t arg3,
                  uint64_t arg4, uint64_t arg5, uint64_t arg6, uint64_t arg7,
                  uint64_t arg8, uint64_t arg9, uint64_t arg10);

/* Front-ends to efi_callX to avoid compiler warnings */
#define _cast64_efi_call0(f) \
  efi_call0(f)
#define _cast64_efi_call1(f,a1) \
  efi_call1(f, (uint64_t)(a1))
#define _cast64_efi_call2(f,a1,a2) \
  efi_call2(f, (uint64_t)(a1), (uint64_t)(a2))
#define _cast64_efi_call3(f,a1,a2,a3) \
  efi_call3(f, (uint64_t)(a1), (uint64_t)(a2), (uint64_t)(a3))
#define _cast64_efi_call4(f,a1,a2,a3,a4) \
  efi_call4(f, (uint64_t)(a1), (uint64_t)(a2), (uint64_t)(a3), (uint64_t)(a4))
#define _cast64_efi_call5(f,a1,a2,a3,a4,a5) \
  efi_call5(f, (uint64_t)(a1), (uint64_t)(a2), (uint64_t)(a3), (uint64_t)(a4), \
            (uint64_t)(a5))
#define _cast64_efi_call6(f,a1,a2,a3,a4,a5,a6) \
  efi_call6(f, (uint64_t)(a1), (uint64_t)(a2), (uint64_t)(a3), (uint64_t)(a4), \
            (uint64_t)(a5), (uint64_t)(a6))
#define _cast64_efi_call7(f,a1,a2,a3,a4,a5,a6,a7) \
  efi_call7(f, (uint64_t)(a1), (uint64_t)(a2), (uint64_t)(a3), (uint64_t)(a4), \
            (uint64_t)(a5), (uint64_t)(a6), (uint64_t)(a7))
#define _cast64_efi_call8(f,a1,a2,a3,a4,a5,a6,a7,a8) \
  efi_call8(f, (uint64_t)(a1), (uint64_t)(a2), (uint64_t)(a3), (uint64_t)(a4), \
            (uint64_t)(a5), (uint64_t)(a6), (uint64_t)(a7), (uint64_t)(a8))
#define _cast64_efi_call9(f,a1,a2,a3,a4,a5,a6,a7,a8,a9) \
  efi_call9(f, (uint64_t)(a1), (uint64_t)(a2), (uint64_t)(a3), (uint64_t)(a4), \
            (uint64_t)(a5), (uint64_t)(a6), (uint64_t)(a7), (uint64_t)(a8), \
            (uint64_t)(a9))
#define _cast64_efi_call10(f,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) \
  efi_call10(f, (uint64_t)(a1), (uint64_t)(a2), (uint64_t)(a3), (uint64_t)(a4), \
             (uint64_t)(a5), (uint64_t)(a6), (uint64_t)(a7), (uint64_t)(a8), \
             (uint64_t)(a9), (uint64_t)(a10))

#define eficall(func, ...)                        \
  __VA_ARG_NSUFFIX__(_cast64_efi_call, __VA_ARGS__) (func , ##__VA_ARGS__)

#endif

#if defined(HAVE_USE_MS_ABI) && !defined(_MSC_EXTENSIONS)
    #define EFI_FUNCTION __attribute__((ms_abi))
#else
    #define EFI_FUNCTION
#endif

#ifdef _MSC_EXTENSIONS
#pragma warning ( disable : 4731 )  // Suppress warnings about modification of EBP
#endif

#endif
