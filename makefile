kernel_SRC = ../src/kernel/*.c

libstdc_SRC = ../src/stdc/src/*.c

Baselib_SRC =	../src/lib/display/*.c ../src/lib/dev/memory/*.c

objs = $(wildcard output/*.o)

font = output/font.bin

ASM_FLAG = -f elf32 -g
CC_FLAG = -c -fno-builtin -ffreestanding -m32 -nostdlib -nostdinc -g -Og -Wall -I ../src/lib -I ../src/stdc -I ../src/kernel/include

all: uefi bios

uefi: _uefi clean

bios: _bios clean

_uefi: kernel.sys
	cp output/kernel.sys iso/x86/EXOS/
	grub-mkrescue iso/x86/ -o exos_uefi.iso -d /usr/lib/grub/x86_64-efi
	#sudo dd if=exos_uefi.iso of=/dev/sdb

_bios: kernel.sys
	cp output/kernel.sys iso/x86/EXOS/
	grub-mkrescue iso/x86/ -o exos_bios.iso -d /usr/lib/grub/i386-pc 
	#sudo dd if=exos_bios.iso of=/dev/sdb

kernel.sys: libfont libstdc Baselib
	cd output && nasm ../src/boot/boot.asm $(ASM_FLAG) -o boot.o
	cd output && cc $(kernel_SRC) $(CC_FLAG)
	ld $(objs) $(font) -o output/kernel.sys -m elf_i386 -e KernelEntry
	objcopy --only-keep-debug output/kernel.sys output/kernel.sym
	objcopy --strip-debug output/kernel.sys

libfont:
	nasm src/lib/display/font.asm $(ASM_FLAG) -o $(font) 

libstdc:
	cd output && cc $(libstdc_SRC) $(CC_FLAG)

Baselib:
	cd output && cc $(Baselib_SRC) $(CC_FLAG)

dd:
	sudo dd if=exos.iso of=/dev/sdb
	reboot

.PHONY: clean

clean:
	rm $(objs)

.PHONY: init

init:
	sudo apt install binutils nasm grub-common grub-efi grub-pc xorriso mtools
