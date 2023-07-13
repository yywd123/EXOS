#pragma once

#include <exos/acpi.hpp>
#include <platform.hpp>

__NAMESPACE_DECL(Drivers::Apic)

/**
 * @brief 初始化apic
 * @param madt acpi madt表
 * @param coreList 指向处理器内核的列表 会为此列表分配内存并初始化
 * @return 检测到的处理器内核数量
 */
uint8_t __INIT
initialize(Acpi::Madt *madt, Platform::MultiProcessor::Core **coreList);

void
writeLApic(uint32_t reg, uint32_t value);

uint32_t
readLApic(uint32_t reg);

void
writeIOApic(uint32_t reg, uint32_t value);

uint32_t
readIOApic(uint32_t reg);

__NAMESPACE_END