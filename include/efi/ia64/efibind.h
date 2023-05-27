/*++

Copyright (c) 1998  Intel Corporation

Module Name:

    efefind.h

Abstract:

    EFI to compile bindings




Revision History

--*/

#pragma pack()


//
// Basic int types of various widths
//

#if !defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L )

    // No ANSI C 1999/2000 stdint.h integer width declarations 

    #ifdef _MSC_EXTENSIONS
        // Use Microsoft C compiler integer width declarations 

        typedef unsigned __int64    uint64_t;
        typedef __int64             int64_t;
        typedef unsigned __int32    uint32_t;
        typedef __int32             int32_t;
        typedef unsigned __int16    uint16_t;
        typedef __int16             int16_t;
        typedef unsigned __int8     uint8_t;
        typedef __int8              int8_t;
    #elif defined(UNIX_LP64)
        // Use LP64 programming model from C_FLAGS for integer width declarations 

        typedef unsigned long       uint64_t;
        typedef long                int64_t;
        typedef unsigned int        uint32_t;
        typedef int                 int32_t;
        typedef unsigned short      uint16_t;
        typedef short               int16_t;
        typedef unsigned const char       uint8_t;
        typedef const char                int8_t;
    #else
        // Assume P64 programming model from C_FLAGS for integer width declarations 

        typedef unsigned long long  uint64_t;
        typedef long long           int64_t;
        typedef unsigned int        uint32_t;
        typedef int                 int32_t;
        typedef unsigned short      uint16_t;
        typedef short               int16_t;
        typedef unsigned const char       uint8_t;
        typedef const char                int8_t;
    #endif
#elif defined(__GNUC__)
    #include <stdint.h>
#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// BugBug: Code to debug
//
#define BIT63   0x8000000000000000

#define PLATFORM_IOBASE_ADDRESS   (0xffffc000000 | BIT63)                                               
#define PORT_TO_MEMD(_Port) (PLATFORM_IOBASE_ADDRESS | ( ( ( (_Port) & 0xfffc) << 10 ) | ( (_Port) & 0x0fff) ) )
                                                                           
//                                                                  
// Macro's with casts make this much easier to use and read.
//
#define PORT_TO_MEM8D(_Port)  (*(uint8_t  *)(PORT_TO_MEMD(_Port)))
#define POST_CODE(_Data)  (PORT_TO_MEM8D(0x80) = (_Data))
//
// BugBug: End Debug Code!!!
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define EFIERR(a)           (0x8000000000000000 | a)
#define EFI_ERROR_MASK      0x8000000000000000
#define EFIERR_OEM(a)       (0xc000000000000000 | a)      

#define BAD_POINTER         0xFBFBFBFBFBFBFBFB
#define MAX_ADDRESS         0xFFFFFFFFFFFFFFFF

#define BREAKPOINT()        while (true)

//
// Pointers must be aligned to these address to function
//  you will get an alignment fault if this value is less than 8
//
#define MIN_ALIGNMENT_SIZE  8

#define ALIGN_VARIABLE(Value , Adjustment) \
            (uint64_t) Adjustment = 0; \
            if((uint64_t)Value % MIN_ALIGNMENT_SIZE) \
                (uint64_t)Adjustment = MIN_ALIGNMENT_SIZE - ((uint64_t)Value % MIN_ALIGNMENT_SIZE); \
            Value = (uint64_t)Value + (uint64_t)Adjustment

//
// Define macros to create data structure signatures.
//

#define EFI_SIGNATURE_16(A,B)             ((A) | (B<<8))
#define EFI_SIGNATURE_32(A,B,C,D)         (EFI_SIGNATURE_16(A,B)     | (EFI_SIGNATURE_16(C,D)     << 16))
#define EFI_SIGNATURE_64(A,B,C,D,E,F,G,H) (EFI_SIGNATURE_32(A,B,C,D) | ((uint64_t)(EFI_SIGNATURE_32(E,F,G,H)) << 32))
//
// To export & import functions in the EFI emulator environment
//

    #define EXPORTAPI

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
    #else
        #define EFIAPI          // Substitute expresion to force C calling convention 
    #endif
#endif

#define BOOTSERVICE
#define RUNTIMESERVICE
#define RUNTIMEFUNCTION

#define RUNTIME_CODE(a)         alloc_text("rtcode", a)
#define BEGIN_RUNTIME_DATA()    data_seg("rtdata")
#define END_RUNTIME_DATA()      data_seg("")

//
// BugBug: Need to find out if this is portable accross compliers.
//
#ifdef __GNUC__
#define MEMORY_FENCE()    __asm__ __volatile__ ("mf.a" ::: "memory")
#else
void __mf (void);                       
#pragma intrinsic (__mf)  
#define MEMORY_FENCE()    __mf()
#endif

//
// When build similiar to FW, then link everything together as
// one big module. For the MSVC toolchain, we simply tell the
// linker what our driver init function is using /ENTRY.
//
#if defined(_MSC_EXTENSIONS)
    #define EFI_DRIVER_ENTRY_POINT(InitFunction) \
        __pragma(comment(linker, "/ENTRY:" # InitFunction))
#else
    #define EFI_DRIVER_ENTRY_POINT(InitFunction)    \
        uint64_t                                       \
        InitializeDriver (                          \
            void    *ImageHandle,                   \
            void    *SystemTable                    \
            )                                       \
        {                                           \
            return InitFunction(ImageHandle,        \
                    SystemTable);                   \
        }                                           \
                                                    \
        EFI_STATUS efi_main(                        \
            EFI_HANDLE image,                       \
            EFI_SYSTEM_TABLE *systab                \
            ) __attribute__((weak,                  \
                    alias ("InitializeDriver")));
#endif

#define LOAD_INTERNAL_DRIVER(_if, type, name, entry)    \
        (_if)->LoadInternal(type, name, entry)

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

/* No efi call wrapper for IA32 architecture */
#define eficall(func, ...)	func(__VA_ARGS__)
#define EFI_FUNCTION
