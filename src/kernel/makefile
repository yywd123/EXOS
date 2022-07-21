ASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

ASM_FLAG = -f elf64
CC_FLAG = -c -fno-builtin -ffreestanding -m64 -mcmodel=large -nostdlib -nostdinc -fPIE -Wall -I ../../lib -I ../../stdc -I ../include

default: __init kernel init OSfunc framebuffer memory acpi
	mv output/* ../../output
	rmdir output
	echo "{<<<<<<<<<<Kernel Core Compiled Success>>>>>>>>>>}"

__init:
	echo "Build Script For EXOS			By yywd_123"
	mkdir output

kernel:
	$(ASM) Entry.asm $(ASM_FLAG) -o output/Entry.o
	cd output && $(CC) ../Kernel.c $(CC_FLAG)

init:
	cd output && $(CC) ../init.c $(CC_FLAG)

OSfunc:
	cd output && $(CC) ../OSfunc.c $(CC_FLAG)

framebuffer:
	cd output && $(CC) ../framebuffer.c $(CC_FLAG)

memory:
	cd output && $(CC) ../memory.c $(CC_FLAG)

acpi:
	cd output && $(CC) ../acpi.c $(CC_FLAG)

.PHONY: clean
clean:
	rm -r output
