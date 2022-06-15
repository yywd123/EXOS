/*  Bitmap Display Library for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-6-12
*/

#include <OSBase.h>

typedef struct
{
  //  File Header
  WORD Magic;
  DWORD FileSize;
  DWORD Reserved;
  DWORD DataOffset;

  //  Info Header
  DWORD InfoHeaderSize;
  DWORD BmpWidth;
  DWORD BmpHeight;
  DWORD BmpPlanes;
  WORD BmpColorDepth;
  DWORD BmpCompression;
  DWORD BmpDataSize;
  DWORD BmpHorizonalReslotion;
  DWORD BmpVerticalReslotion;
  DWORD BmpColorCount;
  DWORD BmpImportantColorCount;
}__attribute__((packed)) BmpInfo;
