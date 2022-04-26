objs = sources/boot/boot.o sources/kernel/OSfunc.o sources/kernel/Kernel.o

uefi: kernel.sys
	cp output/kernel.sys iso/x86/EXOS/
	grub-mkrescue iso/x86/ -o exos.iso -d /usr/lib/grub/x86_64-efi
	rm $(objs)	
	sudo dd if=exos.iso of=/dev/sdb

bios: kernel.sys
	cp output/kernel.sys iso/x86/EXOS/
	grub-mkrescue iso/x86/ -o exos.iso -d /usr/lib/grub/i386-pc 
	rm $(objs)
	sudo dd if=exos.iso of=/dev/sdb

kernel.sys:
	nasm -f elf32 sources/boot/boot.asm -g
	nasm -f elf32 sources/kernel/OSfunc.asm -g
	cc -c -fno-builtin -ffreestanding -m32 -g -Og sources/kernel/Kernel.c -o sources/kernel/Kernel.o -I sources/kernel -I sources/lib -I sources/stdc
	ld $(objs) -o output/kernel.sys -m elf_i386 -e KernelEntry

dd:
	sudo dd if=exos.iso of=/dev/sdb
	reboot
