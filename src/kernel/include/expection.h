#define EXPECTION_DEBUG 0x00
#define EXPECTION_HARDWARE 0x01
#define EXPECTION_KERNEL_FAULT 0x02
#define EXPECTION_UNDEFINED 0xff

void EXOSAPI EXPECTION_HANDLER(int32_t ERRCODE, uint8_t ERRTYPE, bool DUMP)
{

  Vinfo.BackGround_Color = 0xff006080;
  DrawBlock(0, 0, Vinfo.Scrn_height, Vinfo.Scrn_width, Vinfo.BackGround_Color);
  if(!ERRTYPE)
  {
    Vinfo.Cursor_x = 50;
    Vinfo.Cursor_y = 50;
    puts(L"EXOS遇到了一个问题.请尝试重启你的电脑.\n");
    Vinfo.Cursor_x = 50;
    puts(L"你也可以找up求助(doge\n");
    Vinfo.Cursor_x = 50;
    puts(L"up个人空间:https://space.bilibili.com/689917252/\n");
    Vinfo.Cursor_x = Vinfo.Scrn_width - 225;
    Vinfo.Cursor_y = Vinfo.Scrn_height - 50;
    puts(L"错误代码:0x");
    wchar_t ERRStr[17] = {0};
    itol(ERRCODE, ERRStr, L'x');
    puts(ERRStr);
  }
  io_hlt();
}
