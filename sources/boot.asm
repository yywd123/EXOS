;Bootloader for EXOS
;Copyright (C) 2020-2022 yywd_123
;date: 2022-3-30

bits 16
org 0x0

;fat32 header
  jmp short main
  nop
  OEM_ID              db    "EXOS0.1a"
  BytesPerSector      dw    0x200
  SectorsPerClustor   db    0x08
  ReservedSectors     dw    0x0020
  Total_FAT_Table     db    0x02
  MaxRootEntries      dw    0
  NumberOfSectors     dw    0
  DiskType            db    0xf8
  SectorsPerFAT       dw    0
  SectorsPerTrack     dw    0x003f
  Total_Track         dw    0xff
  HiddenSectors       dd    0
  TotalSectors        dd`   0x927c0           ;300MB
  FATSize             dd    (307200 / 4) * 4  ;(DiskSize / ClusterSize) * 4
  Flags               dw    0
  FSVersion           dw    0
  RootDirStart        dd    2
  FSInfo              dw    1
  BackupBootSector    dw    6
  times 12 db 0                               ;Reserved
  DriveNumber         db    0
  ReservedBytes       db    0
  Signature           db    0x29
  VolumeID            dd    0x12345678
  VolumeLabel         db    "EXOSLDR  "
  FSType              db    "FAT32   "

main:
  


