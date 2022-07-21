ASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

ASM_FLAG = -f elf64
CC_FLAG = -c -fno-builtin -ffreestanding -m64 -mcmodel=large -nostdlib -nostdinc -fPIE -Wall -I .. -I ../../lib -I ../../kernel/include

libstdc: __init math string
	mv output/* ../../output
	rmdir output
	echo "{<<<<<<<<<<Standard C Library Compiled Success>>>>>>>>>>}"

__init:
	mkdir output

math:
	cd output && $(CC) ../src/math.c $(CC_FLAG)

string:
	cd output && $(CC) ../src/string.c $(CC_FLAG)

.PHONY: clean
clean:
	rm -r output
