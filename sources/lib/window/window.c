int CreateWindow(WINDOW info)
{
  switch(info.WindowType)
  {
    case 0x00:    //Console Window
      {
        if(info.y + info.h * 16 + 30 > Vinfo.Scrn_height | info.x + info.v * 8 + 4 > Vinfo.Scrn_width) return -1;
        info.IFArea_x = info.x + 2;
        info.IFArea_y = info.y + 52;
        info.IFArea_h = info.h * 16;
        info.IFArea_v = info.v * 8;
        DrawBlock(info.x, info.y + 24, info.h * 16 + 30, info.v * 8 + 4, 0xff666666);               //Background
        DrawBlock(info.x + 2, info.y + 26, 24, info.v * 8, 0x8000457a);                             //TaskBar
        DrawBlock(info.IFArea_x, info.IFArea_y, info.IFArea_h, info.IFArea_v, 0xff000000);          //Console
        return 0;
      }
     break;
    case 0x01:    //Normal Window
     {
        if(info.y + info.h + 30 > Vinfo.Scrn_height | info.x + info.v + 4 > Vinfo.Scrn_width) return -1;
        info.IFArea_x = info.x + 2;
        info.IFArea_y = info.y + 52;
        info.IFArea_h = info.h;
        info.IFArea_v = info.v;
        DrawBlock(info.x, info.y + 24, info.h + 30, info.v + 4, 0xff666666);                        //Background
        DrawBlock(info.x + 2, info.y + 26, 24, info.v, 0x8000457a);                                 //TaskBar
        DrawBlock(info.IFArea_x, info.IFArea_y, info.IFArea_h, info.IFArea_v, 0xffffffff);          //MainInterface
        return 0;
     }
     break;
  }
}
