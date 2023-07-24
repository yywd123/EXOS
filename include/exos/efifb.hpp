#pragma once

#include <display/display.hpp>

__NAMESPACE_DECL(Drivers::EfiFb)

void __INIT
initialize();

void
drawPixel(Display::Vec2D pos, RGBColor color);

void
drawRect(Display::Vec2D pos1, uint32_t w, uint32_t h, RGBColor color);

void
drawRect(Display::Vec2D pos1, Display::Vec2D pos2, RGBColor color);

void
drawRect(Display::BoundingBox boundingBox, RGBColor color);

void
drawLine(Display::Vec2D pos1, Display::Vec2D pos2, RGBColor color);

uint32_t
getWidth();

uint32_t
getHeight();

Display::Vec2D
getSize();

void *
getFramebuffer();

/**
 * @brief 将一块区域内的图像信息移至显存的指定地点(假设这块区域与显存大小一致)
 * @param src 要复制的内容所在区域
 * @param srcOffset 要复制的内容在区域内的偏移
 * @param destination 复制到显存的地点
 */
void
copyToFramebuffer(void *src, Display::Vec2D inOffset, Display::Vec2D destination, Display::Vec2D copySize);

__NAMESPACE_END