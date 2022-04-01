objs = sources/boot.o sources/OSfunc.o sources/Kernel.o 

image: kernel.sys
	cp output/kernel.sys iso/x86/
	grub-mkrescue iso/x86/ -o exos.iso
	rm sources/*.o output/*

kernel.sys:
	nasm -f elf32 sources/boot.asm
	nasm -f elf32 sources/OSfunc.asm
	cc -c -fno-builtin -ffreestanding -m32 sources/*.c -o sources/Kernel.o -I ./sources/
	ld $(objs) -o output/kernel.sys -m elf_i386
