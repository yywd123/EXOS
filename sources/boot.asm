;multiboot header for EXOS
;Copyright (C) 2020-2022 yywd_123
;date: 2022-3-25

section .text
global start, _start
extern KernelInit

start:
_start:
  jmp KernelEntry

  align 8

header_start:
  dd 0xe85250d6     ;multiboot2
  dd 0              ;i386 protected mode
  dd header_end - header_start  ;header length
  dd -(0xe85250d6 + 0 + (header_end - header_start))  ;checksum
  
addr_tag_start:
  db 2
  db 1
  dd addr_tag_end - addr_tag_start
  dd _start
  dd 0x0
  dd 0x0
addr_tag_end:
entry_addr_tag_start:
  db 3
  db 1
  dd entry_addr_tag_end - entry_addr_tag_start
  dd KernelEntry
entry_addr_tag_end:
framefuffer_tag_start:
  db 5
  db 1
  dd framefuffer_tag_end - framefuffer_tag_start
  dd 1024
  dd 768
  dd 32
framefuffer_tag_end:
  ;tag end
  dw 0
  dw 0
  dd 8
header_end:
[bits 32]
KernelEntry:
  mov esp, (stack + 0x4000)

  push 0
  popf

  push ebx
  push eax

  call KernelInit

section .bss
  stack resb  0x4000
