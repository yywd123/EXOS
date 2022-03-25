objs = sources/boot.o sources/OSfunc.o sources/Kernel.o

image: kernel.sys
	cp output/kernel.sys iso/i386/
	grub-mkrescue -o exos.iso iso/i386
	rm output/* sources/*.o 

kernel.sys:
	nasm -f elf64 sources/boot.asm
	nasm -f elf64 sources/OSfunc.asm
	cc -c -fno-builtin -ffreestanding -m64 sources/*.c -o sources/Kernel.o
	ld $(objs) -o output/kernel.sys -m elf_x86_64

.PHONY: clean
clean:
	rm -r output/* sources/*.o
