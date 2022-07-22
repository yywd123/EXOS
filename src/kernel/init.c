/*  System Init Part for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-24
*/

#include <OSBase.h>

extern VideoInfo Vinfo; 

KRNLSTAT EXOSAPI KernelInit(BootInfo *BInfo)
{
  KRNLSTAT InitStat = 0;
  VideoInit(BInfo);
  //GraphicTest();

  DrawBlock(0, 0, Vinfo.Screen_height, Vinfo.Screen_width, Vinfo.BackGround_Color);

  wprintf(L"\nFrameBuffer Address: 0x%x", Vinfo.fbAddress);

  InitSerialPort(COM1);

  printk(LOG_INFO, "Kernel Init Success!!");
  wputs(L"\n[ INFO ] 系统初始化成功!!\n");
  //while(1);

  return InitStat;
}
