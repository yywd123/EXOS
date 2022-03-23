ENTRY(start)

SECTIONS
{
  . = 1M;

  .boot :
  {
    KEEP(*(.kernel_header))
  }

  .text :
  {
    *(.kernel_loader)
  }
}
