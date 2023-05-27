#ifndef GNU_EFI_SETJMP_H
#define GNU_EFI_SETJMP_H

#include "eficompiler.h"
#include "efisetjmp_arch.h"

extern uint64_t setjmp(jmp_buf *env) __attribute__((returns_twice));
extern void longjmp(jmp_buf *env, uint64_t value) __attribute__((noreturn));

#endif /* GNU_EFI_SETJMP_H */
