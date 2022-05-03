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


