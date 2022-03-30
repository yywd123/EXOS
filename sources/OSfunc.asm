section .OSfunc
global io_hlt, LongMode_init

io_hlt:
  hlt
  ret
[bits 32]
LongMode_init:
  mov dword [0x10000], 0x11007
  mov dword [0x10800], 0x11007
  
  mov dword [0x11000], 0x12007
  
  mov dword [0x12000], 0x000083
  mov dword [0x12008], 0x200083
  mov dword [0x12010], 0x400083
  mov dword [0x12018], 0x600083
  mov dword [0x12020], 0x800083
  mov dword [0x12028], 0xa00083

  db 0x66
  lgdt [gdt64_ptr]
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov fs, ax
  mov gs, ax
  mov esp, 0x7c00

  mov eax, cr4
  bts eax, 5
  mov cr4, eax


  mov eax, 0x10000
  mov cr3, eax


  mov ecx, 0x0c0000080
  rdmsr
  bts eax, 8
  wrmsr


  mov eax, cr0
  bts eax, 31
  mov cr0, eax

  mov dword [0xb8000], 0x2f4b2f4f
  ret

section .rodata
  gdt64_base  dq  0x0000000000000000
  gdt64_code  dq  0x0020980000000000
  gdt64_data  dq  0x0000920000000000
  gdt64_ptr:
    dw $ - gdt64_base - 1
    dd gdt64_base
