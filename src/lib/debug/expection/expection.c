void EXOSAPI EXPECTION_HANDLER(uint32_t ERRCODE, uint8_t ERRTYPE, bool DUMP)
{
  Vinfo.BackGround_Color = 0xff0060ff;
  DrawBlock(0, 0, Vinfo.Scrn_height, Vinfo.Scrn_width, Vinfo.BackGround_Color);
  Vinfo.Cursor_x = 125;
  Vinfo.Cursor_y = 50;
  puts(L"EXOS\x9047\x5230\x4e86\x95ee\x9898,\x8bf7\x5c1d\x8bd5\x91cd\x65b0\x542f\x52a8\x7535\x8111.\n");
  Vinfo.Cursor_x = 125;
  puts(L"EXOS ran into a problem.Please try restarting the computer.\n");
  io_hlt();
}
