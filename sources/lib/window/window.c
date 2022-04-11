int CreateWindow(WINDOW info)
{
  switch(info.WindowType)
  {
    case 0x00:
      {
        if(info.y + info.h * 16 + 24 > Vinfo.Scrn_height | info.x + info.v * 8 > Vinfo.Scrn_width) return -1;
        DrawBlock(info.x, info.y + 24, info.h * 16 + 30, info.v * 8 + 4, 0xff666666);               //Background
        DrawBlock(info.x + 2, info.y + 26, 24, info.v * 8, 0x8000457a);                          //TaskBar
        DrawBlock(info.x + 2, info.y + 52, info.h * 16, info.v * 8, 0xff000000);           //Console
        return 0;
      }
     break;
  }
  /*
  DrawBlock(info.x + 24, info.y, info.h, info.v, 0xff666666);
  DrawBlock(info.x + 26, info.y + 2, 24, info.v - 4, 0x8000457a);
  if(!info.WindowType)      //Console
  {
    DrawBlock(info.x + 26, info.y + 30, info.h - 32, info.v - 4, 0xff000000);
  }
  else                      //Normal
  {
    DrawBlock(info.x + 26, info.y + 30, info.h - 32, info.v - 4, 0x80ffffff);
  }*/

}
