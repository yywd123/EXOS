ASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

ASM_FLAG = -f elf64
CC_FLAG = -c -fno-builtin -ffreestanding -m64 -mcmodel=large -nostdlib -nostdinc -fPIE -Wall -I ../src/lib -I ../src/stdc -I ../src/kernel/include

libstdc_SRC = ../src/stdc/src/*.c

Baselib_SRC =	../src/lib/display/*.c

objs = *.o

font = font.bin

link:
	make __link -s

__link: core Baselib libstdc
	cd output && $(LD) $(objs) $(font) -o kernel.elf -T ../src/linker.ld
	cd output && $(OBJCOPY) -I elf64-x86-64 -S -j ".text" -j ".data" -j ".rodata" -j ".bss" -O binary kernel.elf kernel.sys
	echo "{<<<<<<<<<<Kernel Linked Success>>>>>>>>>>}"
	echo "{!!!!!!!!!!Kernel Compiled Success!!!!!!!!!!}"

core:
	make -C src/kernel -s

Baselib:
	make -C src/lib -s

libstdc:
	make -C src/stdc -s

dd:
	sudo dd if=exos.iso of=/dev/sdb
	reboot

.PHONY: clean
clean:
	make __clean -s

__clean:
	rm -r output/*.o output/*.bin output/*.elf src/kernel/output src/lib/output src/stdc/output

.PHONY: run

run: all
	cp output/kernel.sys vm/hda/kernel.sys
	cd vm && qemu-system-x86_64 -s -S -nographic -pflash OVMF.fd -net none -m 64m -hda fat:rw:hda\

.PHONY: InitWorkSpace

InitWorkSpace:
	sudo apt install binutils nasm genisoimage
