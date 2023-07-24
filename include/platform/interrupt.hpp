#ifndef __PLATFORM_H__
#error 请勿直接include这个头文件 请使用platform.h来代替
#endif

#pragma once

__NAMESPACE_DECL(Platform::Interrupt)

void
initialize(MultiProcessor::Core *core);

void
setHandler(uint8_t index, Fn<void, const InterruptFrame *> handler);

__NAMESPACE_END
