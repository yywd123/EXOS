;multiboot header for EXOS
;Copyright (C) 2020-2022 yywd_123
;date: 2022-3-31

Stack_Size  EQU  0x4000

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
  dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))
  
  align 8
  info_request_tag:
    dw 1
    dw 0
    dd info_request_tag_end - info_request_tag
    dd 4            ;request basic_meminfo 
  info_request_tag_end:

  align 8
  addr_tag:
    dw 2
    dw 1
    dd addr_tag_end - addr_tag
    dd header_start
    dd KernelEntry
    dd 0
    dd 0
  addr_tag_end:
      
  align 8
  entry_tag:
    dw 3
    dw 1
    dd entry_tag_end - entry_tag
    dd KernelEntry
  entry_tag_end:

  align 8
  framebuffer_tag:
    dw 5
    dw 1
    dd 20
    dd 1024
    dd 768
    dd 24

  ;tag end
  align 8
  dw 0
  dw 0
  dd 8
header_end:
KernelEntry:
  mov esp, (Stack + Stack_Size)

  push 0
  popf

  push ebx
  push eax

  call KernelInit
  cmp eax, -1
  je error

error:
  hlt
  jmp error
section .bss
Stack  resb  Stack_Size
