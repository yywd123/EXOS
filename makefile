objs = sources/boot/boot.o sources/kernel/OSfunc.o sources/kernel/Kernel.o

image: kernel.sys
	cp output/kernel.sys iso/x86/EXOS/
	grub-mkrescue iso/x86/ -o exos.iso
	rm $(objs) output/*

kernel.sys:
	nasm -f elf32 sources/boot/boot.asm
	nasm -f elf32 sources/kernel/OSfunc.asm
	cc -c -fno-builtin -ffreestanding -m32 sources/kernel/Kernel.c -o sources/kernel/Kernel.o -I ./sources/kernel -I ./sources/lib
	ld $(objs) -o output/kernel.sys -m elf_i386

dd: image
	sudo dd if=exos.iso of=/dev/sdb
	reboot
