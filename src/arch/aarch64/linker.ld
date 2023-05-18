OUTPUT_FORMAT(elf64-littleaarch64)
OUTPUT_ARCH(aarch64)
ENTRY(kernelEntry)

SECTIONS
{
	.	=	0x401000;
	.text BLOCK(4K) : ALIGN(4K)
	{
		_text = .;
		*(.text.entry)
		*(.text)
		_etext = .;
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		_data = .;
		*(.data)		
		_edata = .;
    bss = .;
		*(.bss)
		_ebss = .;
	}

	.rodata BLOCK(4K) : ALIGN(4K)
	{
		_rodata = .;	
		*(.rodata)
		_erodata = .;
	}

	_end = .;
}