global io_hlt

io_hlt:
  hlt
  jmp io_hlt

global Shutdown,Reboot
;I don't know how to shutdown or reboot qwq
Shutdown:


Reboot:
  mov dx, 0x64
  mov al, 0xfe
  out dx, al

global outb, outw, outd
outb:       ;void EXOSAPI outb(uint8_t data, uint16_t port)
  mov al, cl
  out dx, al 

  ret

outw:       ;void EXOSAPI outw(uint16_t data, uint16_t port)
  mov ax, cx
  out dx, ax

  ret

outd:       ;void EXOSAPI outd(uint32_t data, uint16_t port)
  mov eax, ecx
  out dx, eax

  ret

global inb, inw, ind
inb:        ;uint8_t EXOSAPI inb(uint16_t port)
  xor eax, eax
  mov dx, cx
  in al, dx

  ret

inw:        ;uint16_t EXOSAPI inw(uint16_t port)
  xor eax, eax
  mov dx, cx
  in ax, dx

  ret

ind:        ;uint32_t EXOSAPI ind(uint16_t port)
  xor eax, eax
  mov dx, cx
  in eax, dx

  ret
