global io_hlt

io_hlt:
  hlt
  ret

global memcpy
memcpy:     ;void *memcpy(void *target, void *source, uint32_t size);
  ;store registers
  push ebp
  mov ebp, esp

  push esi
  push edi
  push ecx

  ;get params
  mov esi, [ebp + 12]     ;source
  mov edi, [ebp + 8]      ;target
  mov ecx, [ebp + 16]     ;size
  .1:
    cmp ecx, 0
    jz .2
    ;copy byte
    mov al, [ds:esi]
    mov [ds:edi], al

    ;next byte
    inc esi
    inc edi
    
    ;continue to copy
    loop .1

  .2:
    mov eax, [ebp + 8]    ;return target
    
    ;restore registers
    pop ecx
    pop edi
    pop esi
    pop ebp

    ret                   ;function is over

    
