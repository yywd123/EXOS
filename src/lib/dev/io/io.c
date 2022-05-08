void EXOSAPI outb(uint16_t port, uint8_t data)
{
  __asm__ __volatile__( "outb %0, %%dx \n\t"
                        "mfence"
                        ::"a"(data), "d"(port)
                        :"memory");
}

void EXOSAPI outw(uint16_t port, uint16_t data)
{
  __asm__ __volatile__( "outw %0, %%dx \n\t"
                        "mfence"
                        ::"a"(data), "d"(port)
                        :"memory");
}

void EXOSAPI outd(uint16_t port, uint32_t data)
{
  __asm__ __volatile__( "outl %0, %%dx \n\t"
                        "mfence"
                        ::"a"(data), "d"(port)
                        :"memory");
}

uint8_t EXOSAPI inb(uint16_t port)
{
  uint8_t result = 0;
  __asm__ __volatile__( "inb %%dx, %0 \n\t"
                        "mfence"
                        :"=a"(result)
                        :"d"(port)
                        :"memory");
  return result;
}

uint16_t EXOSAPI inw(uint16_t port)
{
  uint16_t result = 0;
  __asm__ __volatile__( "inw %%dx, %0 \n\t"
                        "mfence"
                        :"=a"(result)
                        :"d"(port)
                        :"memory");
  return result;
}

uint32_t EXOSAPI ind(uint16_t port)
{
  uint32_t result = 0;
  __asm__ __volatile__( "inl %%dx, %0 \n\t"
                        "mfence"
                        :"=a"(result)
                        :"d"(port)
                        :"memory");
  return result;
}
