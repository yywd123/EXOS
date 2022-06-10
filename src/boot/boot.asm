;multiboot header for EXOS
;Copyright (C) 2020-2022 yywd_123
;date: 2022-3-31

[bits 32]
Stack_Size    EQU   0x4000

section .text
global KernelEntry
extern GetBootConf, KernelMain
header_start:
  dd 0xe85250d6     ;multiboot2
  dd 0              ;i386 protected mode
  dd header_end - header_start  ;header length
  dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

  align 8
  framebuffer_tag:
    dw 5
    dw 1
    dd 20
    dd 1024
    dd 768
    dd 32

  ;tag end
  align 8
  dw 0
  dw 0
  dd 8
header_end:
KernelEntry:
  ;Init Stack
  mov esp, (Stack + Stack_Size)

  push 0
  popf

  push ebx
  push eax

  call GetBootConf
  cmp eax, 0
  jne GetBootConfFailed  

  add esp, 8

  call KernelMain

halt:
  hlt
  jmp halt

extern EXPECTION_HANDLER
InitFailedCode    EQU     0x01
EXPECTION_NoDump  EQU     0x00
GetBootConfFailed:
  push EXPECTION_NoDump
  push InitFailedCode
  push eax

  call EXPECTION_HANDLER

  add esp, 12 

  hlt
  jmp halt

section .bss
  Stack     resb    Stack_Size
