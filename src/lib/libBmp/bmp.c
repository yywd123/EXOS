#include <OSBase.h>

void DisplayBmp(uint8_t *BmpAddr, uint32_t TargetX, uint32_t TargetY) 
{
  int i, j;
  uint8_t r, g, b;
  int width = *(int*)(BmpAddr + 18);
  int height = *(int*)(BmpAddr + 22);
  int offset = *(int*)(BmpAddr + 10);
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      b = BmpAddr[offset + i * width * 3 + j * 3 + 0];
      g = BmpAddr[offset + i * width * 3 + j * 3 + 1];
      r = BmpAddr[offset + i * width * 3 + j * 3 + 2];
      DrawPixel(TargetX + j, TargetY + height - 1 - i, 0xff000000 + r * 0x10000 + g * 0x100 + b);
    }
  }
}