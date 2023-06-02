#include <efi/efi.h>
#include <drivers/efifb.hpp>

using namespace EXOS::Drivers;
using namespace EXOS::Display;

static RGBColor *framebuffer = nullptr;
static uint32_t width = 0;
static uint32_t height = 0;

static void
drawPixel(uint32_t x, uint32_t y, RGBColor color) {
  if (x < width && y < height) framebuffer[y * width + x] = color;
}

static void
drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, RGBColor color) {
  for (uint32_t i = 0; i < h; ++i) {
    for (uint32_t j = 0; j < w; ++j) {
      if (x + j < width && y + i < height) drawPixel(x + j, y + i, color);
    }
  }
}

DisplayImpl __INIT
*EfiFb::getDisplayImpl() {
  Guid gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP = (EFI_GRAPHICS_OUTPUT_PROTOCOL*)efiLocateProtocol(&gopGuid, nullptr);

  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *modeInfo;
  uint64_t infoSize;
  uint64_t pixelCount = GOP->Mode->Info->PixelsPerScanLine * GOP->Mode->Info->VerticalResolution;
  uint32_t modeIndex = GOP->Mode->Mode;

  for (uint32_t i = 0; i < GOP->Mode->MaxMode; ++i) {
    eficall(GOP->QueryMode, GOP, i, &infoSize, &modeInfo);
    if ((modeInfo->PixelsPerScanLine * modeInfo->VerticalResolution) >= pixelCount) {
      pixelCount = modeInfo->PixelsPerScanLine * modeInfo->VerticalResolution;
      modeIndex = i;
    }
  }
  eficall(GOP->SetMode, GOP, modeIndex);

  GOP = (EFI_GRAPHICS_OUTPUT_PROTOCOL*)efiLocateProtocol(&gopGuid, nullptr);

  framebuffer = (RGBColor*)GOP->Mode->FrameBufferBase;
  width = GOP->Mode->Info->PixelsPerScanLine;
  height = GOP->Mode->Info->VerticalResolution;

  return new(efiAllocatePool(sizeof(DisplayImpl))) DisplayImpl(drawPixel, drawRect);
}