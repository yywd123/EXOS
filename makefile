objs = sources/boot/boot.o sources/kernel/OSfunc.o sources/kernel/Kernel.o

image: kernel.sys
	cp output/kernel.sys iso/x86/EXOS/
	grub-mkrescue iso/x86/ -o exos.iso
	rm $(objs) output/*

kernel.sys:
<<<<<<< HEAD
	nasm -f elf32 sources/boot/boot.asm
	nasm -f elf32 sources/kernel/OSfunc.asm
	cc -c -fno-builtin -ffreestanding -m32 testkernel.c -o sources/kernel/Kernel.o -I ./sources/kernel -I ./sources/lib
	ld $(objs) -o output/kernel.sys -m elf_i386
=======
	nasm -f elf64 sources/boot/boot.asm
	nasm -f elf64 sources/kernel/OSfunc.asm
	cc -c -fno-builtin -ffreestanding -m64 -mcmodel=large sources/kernel/Kernel.c -o sources/kernel/Kernel.o -I ./sources/kernel -I ./sources/lib
	ld $(objs) -o output/kernel.sys -m elf_x86_64 -e KernelEntry32 -w+gnu-elf-extensions
>>>>>>> fb0905c7de18b56b65889eb1fe36ac74c60cd3e3

dd: image
	sudo dd if=exos.iso of=/dev/sdb
	reboot
