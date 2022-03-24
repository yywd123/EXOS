image: kernel.sys
	cp output/kernel.sys iso/i386/
	grub-mkrescue -o exos.iso iso/i386
	rm output/*

kernel.sys:
	nasm -f elf32 sources/*.asm
#cc -c -fno-builtin -ffreestanding sources/*.c
	ld sources/*.o -T sources/linker.ld -o output/kernel.sys -m elf_i386

.PHONY: clean
clean:
	rm -r output/*
