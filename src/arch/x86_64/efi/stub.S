.section .text.init

.global _start
_start:
	lea ImageBase(%rip), %rdi
  movq %rcx, %rsi

	jmp efi_main

.section .reloc
.long	0
.long	0xa
.word	0

