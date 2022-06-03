/*  CPU Library for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-20
*/

void EXOSAPI cpuid(DWORD mode, DWORD eax, DWORD ebx, DWORD ecx, DWORD edx)
{
  __asm__ __volatile__("cpuid"
                      :"=a" (eax),"=b" (ebx),"=c" (ecx),"=d" (edx)
                      :"a" (mode));
}

void EXOSAPI GetCPUBrand(wchar_t *Brand)
{
  uint32_t a = 0, b = 0, c = 0, d = 0;
  uint32_t i = 0, k = 0;

  memset(Brand, 0, 13);

  cpuid(0x80000000,a,b,c,d);
  if(a < 0x80000004)
  {
      printf(L"[ERROR] CPU不支持cpuid指令! status: %x\n", a);
      return;
  }

  for(i=0x80000002;i<=0x80000004;i++)
  {
      cpuid(i,a,b,c,d);
      Brand[k++]=(wchar_t)a;
      Brand[k++]=(wchar_t)b;
      Brand[k++]=(wchar_t)c;
      Brand[k++]=(wchar_t)d;
  }

  printf(L"CPU型号: %s", Brand);

  return;
}
