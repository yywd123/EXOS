ASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

ASM_FLAG = -f elf64
CC_FLAG = -c -fno-builtin -ffreestanding -m64 -mcmodel=large -nostdlib -nostdinc -fPIE -Wall -I ../ -I ../../stdc -I ../../kernel/include

Baselib: __init display devices
	mv output/* ../../output
	rmdir output
	echo "{<<<<<<<<<<Kernel Base Library Compiled Success>>>>>>>>>>}"

__init:
	mkdir output

display: __display_font __display_print

__display_print:
	cd output && $(CC) ../display/print.c $(CC_FLAG)

__display_font:
	$(ASM) display/font.asm $(ASM_FLAG) -o output/font.bin

devices:
	make __devices_keyboard -s
	make __devices_disk -s

__devices_keyboard:
	cd output && $(CC) ../dev/keyboard/keyboard.c $(CC_FLAG)

__devices_disk:
	cd output && $(CC) ../dev/media/disk/IDE.c $(CC_FLAG)

.PHONY: clean
clean:
	rm -r output 
