OUTPUT_FORMAT("elf64-x86-64","elf64-x86-64","elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(kernelEntry)
SECTIONS
{
	.	=	0x401000;
	.text :
	{
		_text = .;
		*(.text)
		_etext = .;
	}

	.data :
	{
		_data = .;
		*(.data)		
		_edata = .;
    bss = .;
		*(.bss)
		_ebss = .;
	}

	.rodata : 
	{
		_rodata = .;	
		*(.rodata)
		_erodata = .;
	}

	_end = .;
}