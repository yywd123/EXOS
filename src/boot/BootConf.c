/*  Get Boot Configure Part for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-6-10
*/

#include <OSBase.h>

extern BootInfo BOOTINFO;
extern VideoInfo Vinfo;

uint8_t asmlinkage GetBootConf(uint32_t magic, uint32_t InfoAddr)
{
  if(magic != MULTIBOOT2_BOOTLOADER_MAGIC || InfoAddr & 7) return 1;

  struct multiboot_tag *tag;

  bool IsMemMapExist = false;

  for(tag = (struct multiboot_tag *)(InfoAddr + 8);
      tag->type != MULTIBOOT_TAG_TYPE_END;
      tag = (struct multiboot_tag *)((uint8_t *)tag + ((tag->size + 7) & ~7)))
  {
    switch(tag->type)
    {
      case MULTIBOOT_TAG_TYPE_CMDLINE:
        {
          // nothing
        }break;
      case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
        {
          // nothing
        }break;
      case MULTIBOOT_TAG_TYPE_MODULE:
        {
          BOOTINFO.ModInfo = (struct multiboot_tag_module *)tag;
        }break;
      case MULTIBOOT_TAG_TYPE_BOOTDEV:
        {
          // nothing
        }break;
      case MULTIBOOT_TAG_TYPE_MMAP:
        {
          BOOTINFO.MemMap = (struct multiboot_tag_mmap *)tag;
          IsMemMapExist = true;
        }break;
      case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
          Vinfo.bpp = ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_bpp;
          Vinfo.pitch = ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_pitch;
          Vinfo.Scrn_width = ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_width;
          Vinfo.Scrn_height = ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_height;
          Vinfo.BackGround_Color = 0xff0080bb;
          Vinfo.ForeGround_Color = 0xffffffff;
          Vinfo.fb = (void *)(unsigned long)((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_addr;
          Vinfo.fb_addr = (uint32_t)((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_addr;

          Vinfo.Cursor_x = 0;
          Vinfo.Cursor_y = 0;

          DrawBlock(0, 0, ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_height, 
              ((struct multiboot_tag_framebuffer *)tag)->common.framebuffer_width, Vinfo.BackGround_Color);                 //Background
        }break;
    }
  }

  if(!IsMemMapExist) return 2;

  GraphicTest();

  return 0;
}
