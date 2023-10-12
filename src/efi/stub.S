.section .inittext, "ax"

.global _start
_start:
	mov %rcx, %rdi
	mov %rdx, %rsi
	
	lea efiEntry(%rip), %rax
	jmp *%rax

.section .reloc
.long	0
.long	0xa
.word	0

