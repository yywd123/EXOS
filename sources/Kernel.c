void io_hlt();
void LongMode_init();

void KernelEntry();

void KernelEntry()
{
  LongMode_init();
  io_hlt();
}
