void EXOSAPI EXPECTION_HANDLER(int32_t ERRCODE, uint8_t ERRTYPE, bool DUMP)
{
  Vinfo.BackGround_Color = 0xff006080;
  DrawBlock(0, 0, Vinfo.Scrn_height, Vinfo.Scrn_width, Vinfo.BackGround_Color);
  if(!ERRTYPE)
  {
    wchar_t *ERRMSG = 0;
    switch(ERRTYPE)
    {
      case EXPECTION_INIT_FAILURE:
        {
          switch(ERRCODE)
          {
            case INIT_FAILED_NO_MMAP:
            {
              ERRMSG = L"没有找到内存分布图";
            }break;

            default:
            {
              ERRMSG = L"未定义的错误";
            }
          }
        }break;
    }
    printf(L"EXOS遇到了一个问题.请尝试重启你的电脑.\n"
           L"你也可以找up求助(doge\n"
           L"up个人空间:https://space.bilibili.com/689917252/\n"
           L"错误代码:0x%x (%s)\n", ERRCODE, ERRMSG);
  }
  else
  {
    printf(L"调试性崩溃\n"
           L"错误代码:0x%x\n"
           L"由于该错误有可能为用户定义,或者是用于系统调试,此错误代码可能并没有一个准确的定义\n",
           ERRCODE);
  }
  io_hlt();
}
