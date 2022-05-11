C_SRC = src/kernel/Kernel.c
objs = src/boot/boot.o src/kernel/OSfunc.o src/kernel/Kernel.o

ASM_FLAG = -f elf32 -g
CC_FLAG = -c -fno-builtin -ffreestanding -m32 -nostdlib -nostdinc -g -Og -Wall -I src/kernel -I src/lib -I src/stdc

uefi: kernel.sys
	cp output/kernel.sys iso/x86/EXOS/
	grub-mkrescue iso/x86/ -o exos_uefi.iso -d /usr/lib/grub/x86_64-efi
	rm $(objs)	
	sudo dd if=exos_uefi.iso of=/dev/sdb

bios: kernel.sys
	cp output/kernel.sys iso/x86/EXOS/
	grub-mkrescue iso/x86/ -o exos_bios.iso -d /usr/lib/grub/i386-pc 
	rm $(objs)
	sudo dd if=exos_bios.iso of=/dev/sdb

kernel.sys:
	nasm src/boot/boot.asm $(ASM_FLAG)
	nasm src/kernel/OSfunc.asm $(ASM_FLAG)
	cc $(C_SRC) -o src/kernel/Kernel.o $(CC_FLAG)
	ld $(objs) -o output/kernel.sys -m elf_i386 -e KernelEntry
	objcopy --only-keep-debug output/kernel.sys output/kernel.sym
	objcopy --strip-debug output/kernel.sys

dd:
	sudo dd if=exos.iso of=/dev/sdb
	reboot
