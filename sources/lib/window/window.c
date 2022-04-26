#define WINDOW_TYPE_CONSOLE 0x00
#define WINDOW_TYPE_NORMAL 0x01

int CreateWindow(WINDOW info)
{
  switch(info.WindowType)
  {
    case WINDOW_TYPE_CONSOLE:     //Console Window
      {
        if(info.y + info.h * 16 + 30 > Vinfo.Scrn_height | info.x + info.v * 8 + 4 > Vinfo.Scrn_width) return -1;
        info.CArea_x = info.x + 2;
        info.CArea_y = info.y + 26;
        info.CArea_h = 24;
        info.CArea_v = info.v * 8;
        info.IFArea_x = info.x + 2;
        info.IFArea_y = info.CArea_y + info.CArea_h + 4;
        info.IFArea_h = info.h * 16;
        info.IFArea_v = info.v * 8;
        DrawBlock(info.x, info.y + 24, info.IFArea_h + 32, info.IFArea_v + 4, 0xff666666);          //Background
        DrawBlock(info.CArea_x, info.CArea_y, info.CArea_h, info.CArea_v, 0x8000457a);                   //TaskBar
        DrawBlock(info.IFArea_x, info.IFArea_y, info.IFArea_h, info.IFArea_v, 0xff000000);          //Console
        return 0;
      }
     break;
    case WINDOW_TYPE_NORMAL:      //Normal Window
     {
        if(info.y + info.h + 30 > Vinfo.Scrn_height | info.x + info.v + 4 > Vinfo.Scrn_width) return -1;
        info.CArea_x = info.x + 2;
        info.CArea_y = info.y + 26;
        info.CArea_h = 24;
        info.CArea_v = info.v;
        info.IFArea_x = info.x + 2;
        info.IFArea_y = info.y + 54;
        info.IFArea_h = info.h;
        info.IFArea_v = info.v;
        DrawBlock(info.x, info.y + 24, info.IFArea_h + 32, info.IFArea_v + 4, 0xff666666);                 //Background
        DrawBlock(info.CArea_x, info.CArea_y, info.CArea_h, info.CArea_v, 0x8000457a);                     //TaskBar
        DrawBlock(info.IFArea_x, info.IFArea_y, info.IFArea_h, info.IFArea_v, 0xffffffff);            //MainInterface
        return 0;
     }
     break;
  }
}
