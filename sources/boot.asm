;multiboot header for EXOS
;Copyright (C) 2020-2022 yywd_123
;date: 2022-3-9

Stack_Size  EQU  0x4000

section .text
global start, _start
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
  framebuffer_tag:
    dw 5
    dw 1
    dd framebuffer_tag_end - framebuffer_tag
    dd 1024
    dd 768
    dd 24
  framebuffer_tag_end:
      
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

section .bss
Stack  resb  Stack_Size