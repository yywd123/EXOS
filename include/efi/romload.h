#ifndef _EFI_ROMLOAD_H
#define _EFI_ROMLOAD_H

#define ROM_SIGNATURE 0xaa55
#define PCIDS_SIGNATURE "PCIR"
#pragma pack(push)
#pragma pack(1)
typedef struct 
{
    uint8_t    Pcids_Sig[4];
    uint16_t  VendId;
    uint16_t  DevId;
    uint16_t  Vpd_Off;
    uint16_t  Size;
    uint8_t Rev;
    uint8_t Class_Code[3];
    uint16_t  Image_Len;
    uint16_t  Rev_Lvl;
    uint8_t Code_Type;
    uint8_t Indi;
    uint16_t  Rsvd;
}PciDataStructure;
typedef struct
{
    uint16_t Size;
    uint32_t Header_Sig;
    uint16_t SubSystem;
    uint16_t MachineType;
    uint8_t  Resvd[10];
    uint16_t EfiOffset;
}ArchData;
typedef struct 
{
    uint16_t Rom_Sig;
    ArchData Arch_Data;
    uint16_t Pcids_Off;
    uint8_t resvd[38];
}RomHeader;
#pragma pack(pop)

#endif
