#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
#include "x86_64/efibind.h"
#elif defined(_M_IA64) || defined(__ia64__)
#include "ia64/efibind.h"
#elif defined (_M_ARM64) || defined(__aarch64__)
#include "aarch64/efibind.h"
#elif defined (_M_MIPS64) || defined(__mips64__) || defined(__mips64)
#include "mips64el/efibind.h"
#else
#error 不支持的架构
#endif

#include "efidef.h"
#include "efidevp.h"
#include "efierr.h"
#include "efiprot.h"
#include "eficon.h"

#include "efiapi.h"
#include "eficompiler.h"
#include "efilib.h"

#ifdef __cplusplus
}
#endif
