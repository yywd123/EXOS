#pragma once

#include "efidef.hpp"

__NAMESPACE_DECL(EFI)

#if defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
#include "x86_64/efibind.hpp"
#elif defined(_M_IA64) || defined(__ia64__)
#include "ia64/efibind.hpp"
#elif defined(_M_ARM64) || defined(__aarch64__)
#include "aarch64/efibind.hpp"
#elif defined(_M_MIPS64) || defined(__mips64__) || defined(__mips64)
#include "mips64el/efibind.hpp"
#else
#error 不支持的架构
#endif

#include "eficon.hpp"
#include "efidevp.hpp"
#include "efierr.hpp"
#include "efiprot.hpp"

#include "efiapi.hpp"
#include "eficompiler.hpp"
#include "efilib.hpp"

__NAMESPACE_END