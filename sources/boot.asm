;multiboot header for EXOS
;Copyright (C) 2020-2022 yywd_123
;date: 2022-3-9

section .kernel_header
header_start:
  dd 0xe85250d6     ;multiboot2
  dd 0              ;i386 protected mode
  dd header_end - header_start  ;header length
  dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))
  
  ;tag end
  dq 0
  dd 8
header_end:
