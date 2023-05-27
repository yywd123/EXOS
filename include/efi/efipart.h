#ifndef _EFI_PART_H
#define _EFI_PART_H

/*++

Copyright (c) 1998  Intel Corporation

Module Name:

    efipart.h
    
Abstract:   
    Info about disk partitions and Master Boot Records




Revision History

--*/

//
//
//

#define EFI_PARTITION   0xef
#define MBR_SIZE        512

#pragma pack(1)

typedef struct {
    uint8_t       BootIndicator;
    uint8_t       StartHead;
    uint8_t       StartSector;
    uint8_t       StartTrack;
    uint8_t       OSIndicator;
    uint8_t       EndHead;
    uint8_t       EndSector;
    uint8_t       EndTrack;
    uint8_t       StartingLBA[4];
    uint8_t       SizeInLBA[4];
} MBR_PARTITION_RECORD;

#define EXTRACT_UINT32(D) (uint32_t)(D[0] | (D[1] << 8) | (D[2] << 16) | (D[3] << 24))

#define MBR_SIGNATURE           0xaa55
#define MIN_MBR_DEVICE_SIZE     0x80000
#define MBR_ERRATA_PAD          0x40000 // 128 MB

#define MAX_MBR_PARTITIONS  4   
typedef struct {
    uint8_t                   BootStrapCode[440];
    uint8_t                   UniqueMbrSignature[4];
    uint8_t                   Unknown[2];
    MBR_PARTITION_RECORD    Partition[MAX_MBR_PARTITIONS];
    uint16_t                  Signature;
} MASTER_BOOT_RECORD;
#pragma pack()


#endif
