#pragma once

#define __attr(x, ...) __attribute__((x, __VA_ARGS__))

#define __INIT __attr(section(".inittext"))
#define __INITDATA __attr(section(".initdata"))

#define __NAKEDFUNC __attr(naked);