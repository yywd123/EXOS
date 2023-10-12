#pragma once

#define __attr(x, ...) __attribute__((x, ##__VA_ARGS__))
#define __packed __attr(packed)
#define __aligned(x) __attr(aligned(x))

#define __INIT __attr(section(".inittext"))
#define __INITDATA __attr(section(".initdata"))

#define __NAKEDFUNC __attr(naked);