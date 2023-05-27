#ifndef _EFI_FS_H
#define _EFI_FS_H

/*++

Copyright (c) 1998  Intel Corporation

Module Name:

    efifs.h

Abstract:

    EFI File System structures



Revision History

--*/


//
// EFI Partition header (normaly starts in LBA 1)
//

#define EFI_PARTITION_SIGNATURE         0x5053595320494249
#define EFI_PARTITION_REVISION          0x00010001
#define MIN_EFI_PARTITION_BLOCK_SIZE    512
#define EFI_PARTITION_LBA               1

typedef struct _EFI_PARTITION_HEADER {
    EFI_TABLE_HEADER    Hdr;
    uint32_t              DirectoryAllocationNumber;
    uint32_t              BlockSize;
    EFI_LBA             FirstUsableLba;
    EFI_LBA             LastUsableLba;
    EFI_LBA             UnusableSpace;
    EFI_LBA             FreeSpace;
    EFI_LBA             RootFile;
    EFI_LBA             SecutiryFile;
} EFI_PARTITION_HEADER;


//
// File header
//

#define EFI_FILE_HEADER_SIGNATURE   0x454c494620494249
#define EFI_FILE_HEADER_REVISION    0x00010000
#define EFI_FILE_STRING_SIZE        260

typedef struct _EFI_FILE_HEADER {
    EFI_TABLE_HEADER    Hdr;
    uint32_t              Class;
    uint32_t              LBALOffset;
    EFI_LBA             Parent;
    uint64_t              FileSize;
    uint64_t              FileAttributes;
    EFI_TIME            FileCreateTime;
    EFI_TIME            FileModificationTime;
    EFI_GUID            VendorGuid;
    const unsigned short              FileString[EFI_FILE_STRING_SIZE];
} EFI_FILE_HEADER;


//
// Return the file's first LBAL which is in the same
// logical block as the file header
//

#define EFI_FILE_LBAL(a)    ((EFI_LBAL *) (((const char *) (a)) + (a)->LBALOffset))

#define EFI_FILE_CLASS_FREE_SPACE   1
#define EFI_FILE_CLASS_EMPTY        2
#define EFI_FILE_CLASS_NORMAL       3


//
// Logical Block Address List - the fundemental block
// description structure
//

#define EFI_LBAL_SIGNATURE      0x4c41424c20494249
#define EFI_LBAL_REVISION       0x00010000

typedef struct _EFI_LBAL {
    EFI_TABLE_HEADER    Hdr;
    uint32_t              Class;
    EFI_LBA             Parent;
    EFI_LBA             Next;
    uint32_t              ArraySize;
    uint32_t              ArrayCount;
} EFI_LBAL;

// Array size 
#define EFI_LBAL_ARRAY_SIZE(lbal,offs,blks)  \
        (((blks) - (offs) - (lbal)->Hdr.HeaderSize) / sizeof(EFI_RL))

//
// Logical Block run-length
//

typedef struct {
    EFI_LBA     Start;
    uint64_t      Length;
} EFI_RL;

//
// Return the run-length structure from an LBAL header
//

#define EFI_LBAL_RL(a)      ((EFI_RL*) (((const char *) (a)) + (a)->Hdr.HeaderSize))

#endif

