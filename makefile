objs = sources/loader.o sources/OSfunc.o sources/Kernel.o 

kernel.sys:
	nasm sources/boot.asm
	nasm -f elf64 sources/loader.asm
	nasm -f elf64 sources/OSfunc.asm
	cc -c -fno-builtin -ffreestanding -m64 sources/*.c -o sources/Kernel.o
	ld $(objs) -o output/kernel.sys -m elf_x86_64

.PHONY: clean
clean:
	rm -r output/* sources/*.o
