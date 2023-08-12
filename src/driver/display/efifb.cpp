#include <efi/efi.hpp>
#include <exos/efifb.hpp>
#include <utils/math.hpp>
#include <mm/paging.hpp>
#include <exos/logger.hpp>

USE(EXOS::Drivers);

static RGBColor *framebuffer = nullptr;
static Display::Vec2D framebufferSize;

__NAMESPACE_DECL(Drivers::EfiFb)

void __INIT
initialize() {
	UUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI::EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = (EFI::EFI_GRAPHICS_OUTPUT_PROTOCOL *)EFI::locateProtocol(&gopGuid, nullptr);

	EFI::EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *modeInfo;
	uint64_t infoSize;
	uint64_t pixelCount = gop->Mode->Info->PixelsPerScanLine * gop->Mode->Info->VerticalResolution;
	uint32_t modeIndex = gop->Mode->Mode;

	for(uint32_t i = 0; i < gop->Mode->MaxMode; ++i) {
		eficall(gop->QueryMode, gop, i, &infoSize, &modeInfo);
		if((modeInfo->PixelsPerScanLine * modeInfo->VerticalResolution) >= pixelCount) {
			pixelCount = modeInfo->PixelsPerScanLine * modeInfo->VerticalResolution;
			modeIndex = i;
		}
	}
	eficall(gop->SetMode, gop, modeIndex);

	gop = (EFI::EFI_GRAPHICS_OUTPUT_PROTOCOL *)EFI::locateProtocol(&gopGuid, nullptr);

	framebuffer = (RGBColor *)gop->Mode->FrameBufferBase;
	framebufferSize.x = gop->Mode->Info->PixelsPerScanLine;
	framebufferSize.y = gop->Mode->Info->VerticalResolution;
}

void
drawPixel(Display::Vec2D pos, RGBColor color) {
	if(!framebuffer) return;
	if(pos.x < framebufferSize.x && pos.y < framebufferSize.y) framebuffer[pos.y * framebufferSize.x + pos.x] = color;
}

void
drawRect(Display::Vec2D pos, uint32_t w, uint32_t h, RGBColor color) {
	if(!framebuffer) return;
	uint32_t x = pos.x;
	uint32_t y = pos.y;

	for(uint32_t i = 0; i < h; ++i) {
		for(uint32_t j = 0; j < w; ++j) {
			if(x + j < framebufferSize.x && y + i < framebufferSize.y) drawPixel({x + j, y + i}, color);
		}
	}
}

void
drawRect(Display::Vec2D pos1, Display::Vec2D pos2, RGBColor color) {
	if(!framebuffer) return;
	uint32_t x = __min(pos1.x, pos2.x);
	uint32_t y = __min(pos1.y, pos2.y);
	uint32_t w = __max(pos1.x, pos2.x) - x;
	uint32_t h = __max(pos1.y, pos2.y) - y;

	for(uint32_t i = 0; i < h; ++i) {
		for(uint32_t j = 0; j < w; ++j) {
			if(x + j < framebufferSize.x && y + i < framebufferSize.y) drawPixel({x + j, y + i}, color);
		}
	}
}

void
drawRect(Display::BoundingBox boundingBox, RGBColor color) {
	if(!framebuffer) return;
	drawRect(Display::Vec2D{boundingBox.x1, boundingBox.y1}, Display::Vec2D{boundingBox.x2, boundingBox.y2}, color);
}

void
drawLine(Display::Vec2D pos1, Display::Vec2D pos2, RGBColor color) {
	if(!framebuffer) return;
	int32_t x1 = pos1.x;
	int32_t y1 = pos1.y;
	int32_t x2 = pos2.x;
	int32_t y2 = pos2.y;

	// https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C++
	const bool steep = (__abs(y2 - y1) > __abs(x2 - x1));
	if(steep) {
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if(x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	const int32_t dx = x2 - x1;
	const int32_t dy = __abs(y2 - y1);

	int32_t error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	const int maxX = (int)x2;

	for(int x = (int)x1; x <= maxX; x++) {
		if(steep) {
			drawPixel({(uint32_t)y, (uint32_t)x}, color);
		} else {
			drawPixel({(uint32_t)x, (uint32_t)y}, color);
		}

		error -= dy;
		if(error < 0) {
			y += ystep;
			error += dx;
		}
	}
}

uint32_t
getWidth() {
	return framebufferSize.x;
}

uint32_t
getHeight() {
	return framebufferSize.y;
}

Display::Vec2D
getSize() {
	return framebufferSize;
}

void *
getFramebuffer() {
	return framebuffer;
}

void
copyToFramebuffer(void *src, Display::Vec2D srcOffset, Display::Vec2D destination, Display::Vec2D copySize) {
	if(!framebuffer) return;
	uint32_t *srcFramebuffer = (uint32_t *)src;

	for(uint32_t i = 0; i < copySize.y; ++i) {
		for(uint32_t j = 0; j < copySize.x; ++j) {
			if(destination.x + j < framebufferSize.x && destination.y + i < framebufferSize.y)
				drawPixel(Display::Vec2D{j, i} + destination, srcFramebuffer[(srcOffset.y + i) * framebufferSize.x + srcOffset.x + j]);
		}
	}
}

__NAMESPACE_END