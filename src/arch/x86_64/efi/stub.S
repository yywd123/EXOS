.section .inittext

.global _start
_start:
	lea _imageBase(%rip), %rdi
	lea _initend(%rip), %rsi
  xchg %rcx, %rdx
	
	jmp efi_main

.section .reloc
.long	0
.long	0xa
.word	0

