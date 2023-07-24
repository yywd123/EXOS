.section .inittext, "ax"

.global _start
_start:
	mov %rcx, %rdi
	mov %rdx, %rsi
	
	jmp efiEntry

.section .reloc
.long	0
.long	0xa
.word	0

