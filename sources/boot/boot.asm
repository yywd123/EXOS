;multiboot header for EXOS
;Copyright (C) 2020-2022 yywd_123
;date: 2022-3-31

[bits 32]
Stack_Size  EQU  0x4000

section .text
global KernelEntry32
extern KernelInit
header_start:
  dd 0xe85250d6     ;multiboot2
  dd 0              ;i386 protected mode
  dd header_end - header_start  ;header length
  dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))
<<<<<<< HEAD
=======
  
  align 8
  info_request_tag:
    dw 1
    dw 0
    dd 12
    dd 4            ;request basic_meminfo 
>>>>>>> fb0905c7de18b56b65889eb1fe36ac74c60cd3e3

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
<<<<<<< HEAD
KernelEntry:
  ;Init Stack
  mov esp, (Stack + Stack_Size)
=======
>>>>>>> fb0905c7de18b56b65889eb1fe36ac74c60cd3e3

extern Addr, magic
KernelEntry32:
  mov [Addr], ebx
  mov [magic], eax

  ;Enable PAE
  mov eax, cr4
  or eax, 1<< 5
  mov cr4, eax

  ;Set PageTable
  ;p3_table
  mov eax, p3_table
  or eax, 0b11
  mov [p4_table], eax
  ;p2_table
  mov eax, p2_table
  or eax, 0b11
  mov [p3_table], eax
  ;p1_table
  mov eax, p1_table
  or eax, 0b11
  mov [p2_table], eax
  ;fill pagetable
  mov eax, p1_table
  mov ebx, 0b11
  mov ecx, 512

.fill_pt:
    mov [eax], ebx
    add eax, 8
    add ebx, 4096
    loop .fill_pt

<<<<<<< HEAD
  push ebx
  call KernelInit

  hlt

error:
  hlt
  jmp error

section .bss
  Stack  resb  Stack_Size
  
=======
  ;write cr3 register
  mov eax, p4_table
  mov cr3, eax

  ;switch to longmode(64bits)
  mov ecx, 0xc0000080
  rdmsr
  or eax, 1 << 8
  wrmsr

  ;enable paging
  mov eax, cr0
  or eax, 1 << 31
  or eax, 1 << 16
  mov cr0,eax

  ;load gdt
  lgdt [gdt64_ptr]

  ;jump to longmode part
  jmp gdt64.code:KernelEntry64

section .bss
  ;Page Table
  align 4096
  p4_table    resb    4096
  p3_table    resb    4096
  p2_table    resb    4096
  p1_table    resb    4096
  
  Stack       resb    Stack_Size

section .data
  ;gdt64
  align 16
  gdt64:
    .null:
      dw  0xffff
      dw  0
      dd  0
    .code:
      dw  0
      dw  0
      db  0
      db  0x9a
      db  0x20
      db  0
    .data:
      dw  0
      dw  0
      db  0
      db  0x92
      dw  0
    .user_code:
      dw  0
      dw  0
      db 0xfa
      db 0x20
      db  0
    .user_data:
      dw  0
      dw  0
      db  0
      db  0xf2
      dw  0
  gdt64_ptr:
    dw  $ - gdt64 -1
    dq  gdt64

[bits 64]
section .text
global KernelEntry64
KernelEntry64:
  mov rax, 0x10
  mov ds, rax
  mov es, rax
  mov fs, rax
  mov ss, rax

  mov rsp, (Stack + Stack_Size)
>>>>>>> fb0905c7de18b56b65889eb1fe36ac74c60cd3e3
