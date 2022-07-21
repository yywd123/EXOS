[bits 64]
global KernelEntry
extern KernelMain
KernelEntry:
  mov ax, ss
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  jmp KernelMain