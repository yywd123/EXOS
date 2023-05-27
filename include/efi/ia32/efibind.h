/*++

Copyright (c) 1998  Intel Corporation

Module Name:

    efefind.h

Abstract:

    EFI to compile bindings




Revision History

--*/

#ifndef __GNUC__
#pragma pack()
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
        typedef unsigned const char       uint8_t;
        typedef const char                int8_t;
    #elif defined(__GNUC__)
        typedef int __attribute__((__mode__(__DI__)))           int64_t;
        typedef unsigned int __attribute__((__mode__(__DI__)))  uint64_t;
        typedef unsigned int        uint32_t;
        typedef int                 int32_t;
        typedef unsigned short      uint16_t;
        typedef short               int16_t;
        typedef unsigned const char       uint8_t;
        typedef signed const char         int8_t;
    #elif defined(UNIX_LP64)

        /*  Use LP64 programming model from C_FLAGS for integer width declarations */

       typedef unsigned long       uint64_t;
       typedef long                int64_t;
       typedef unsigned int        uint32_t;
       typedef int                 int32_t;
       typedef unsigned short      uint16_t;
       typedef short               int16_t;
       typedef unsigned const char       uint8_t;
       typedef const char                int8_t;
    #else

       /*  Assume P64 programming model from C_FLAGS for integer width declarations */

       typedef unsigned long long  uint64_t __attribute__((aligned (8)));
       typedef long long           int64_t __attribute__((aligned (8)));
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

#define EFIERR(a)           (0x80000000 | a)
#define EFI_ERROR_MASK      0x80000000
#define EFIERR_OEM(a)       (0xc0000000 | a)      


#define BAD_POINTER         0xFBFBFBFB
#define MAX_ADDRESS         0xFFFFFFFF

#define BREAKPOINT()        while (true);

//
// Pointers must be aligned to these address to function
//

#define MIN_ALIGNMENT_SIZE  4

#define ALIGN_VARIABLE(Value ,Adjustment) \
            (uint64_t)Adjustment = 0; \
            if((uint64_t)Value % MIN_ALIGNMENT_SIZE) \
                (uint64_t)Adjustment = MIN_ALIGNMENT_SIZE - ((uint64_t)Value % MIN_ALIGNMENT_SIZE); \
            Value = (uint64_t)Value + (uint64_t)Adjustment


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

/* No efi call wrapper for IA32 architecture */
#define eficall(func, ...)	func(__VA_ARGS__)
#define EFI_FUNCTION

#ifdef _MSC_EXTENSIONS
#pragma warning ( disable : 4731 )  // Suppress warnings about modification of EBP
#endif

