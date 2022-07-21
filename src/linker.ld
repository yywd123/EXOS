OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(KernelEntry)
SECTIONS
{
    .text : 
    {
        _text = .;
        Entry.o(.text);
        *(.text)
        _etext = .;
    }

    . = ALIGN(8);

    .data : 
    {
        _data = .;
        Entry.o(.data);
        *(.data)
        _edata = .;
    } 

    .rodata : 
    {
        _rodata = .;
        *(.data)
        _erodata = .;
    } 

    .bss : 
    {
        _bss = .;
        *(.bss)
        _ebss = .;
    }  

    _end = .;
}