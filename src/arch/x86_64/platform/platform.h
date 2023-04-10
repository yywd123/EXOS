#pragma once

#include <utils>

#define SEGFLAG_RW BIT(1)
#define SEGFLAG_EXCUTEABLE BIT(3)

typedef enum {
  interrupt = 0x8e,
  trap = 0x8f
} IntrType;

typedef struct {
  uint64_t rax;
  uint64_t rbx;
  uint64_t rcx;
  uint64_t rdx;
  uint64_t rsi;
  uint64_t rdi;
  uint64_t rbp;
  uint64_t r8;
  uint64_t r9;
  uint64_t r10;
  uint64_t r11;
  uint64_t r12;
  uint64_t r13;
  uint64_t r14;
  uint64_t r15;
} __attribute__((packed)) RegisterFrame;

typedef struct {
  RegisterFrame regs;
  uint64_t irqIndex;
  uint64_t errCode;
  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
} __attribute__((packed)) IntrFrame;

void makeGDTEntry(uint8_t index, unsigned privilege, unsigned flag);
void makeGDTSSEntry(uint8_t index, void *base, uint32_t limit, uint8_t flag);
void registerInterrupt(uint8_t index, uintptr_t entryPoint, uint16_t selector, uint8_t ist, IntrType type);
void loadDT();

