#ifndef __PLATFORM_H__
#error 请勿直接include这个头文件 请使用platform.h来代替
#endif

#pragma once

#include <exos/acpi.hpp>

__NAMESPACE_DECL(Drivers::Apic)

typedef enum {
	CMCI = 0,
	Timer = 3,
	ThermalSensor,
	PerformanceMonitoringCounters,
	LINT0,
	LINT1,
	Error
} LVTIndex;

#define lvt_iter(x) \
	for(LVTIndex lvt : {CMCI, Timer, ThermalSensor, PerformanceMonitoringCounters, LINT0, LINT1, Error}) x

typedef struct {
	uint8_t vector;
	uint8_t deliverMode : 3;
	uint8_t destinationMode : 1;
	uint8_t deliverStatus : 1;
	uint8_t polarity : 1;
	uint8_t irr : 1;
	uint8_t triggerMode;
	uint8_t interruptMask : 1;
	uint16_t reserved0 : 15;

	union {
		struct {
			uint32_t reserved1 : 24;
			uint8_t physicalDestination : 4;
			uint8_t reserved2;
		} physical;

		struct {
			uint32_t reserved1 : 24;
			uint8_t logicalDestination;
		} logical;
	} destination;
} IOApicRTE;

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
writeLApicLVT(LVTIndex index, uint32_t value);

uint32_t
readLApicLVT(LVTIndex index);

void
maskLApicLVT(LVTIndex index, bool enable);

void
lApicAck();

void
writeIOApic(uint8_t reg, uint32_t value);

uint32_t
readIOApic(uint8_t reg);

void
writeIOApicRTE(uint8_t index, uint64_t value);

uint64_t
readIOApicRTE(uint8_t index);

void
registerIOApicRTE(uint8_t index, IOApicRTE *entry);

void
unregisterIOApicRTE(uint8_t index);

void
maskIOApicRTE(uint8_t index, bool enable);

void
ioApicEdgeAck();

void
ioApicLevelAck(uint8_t irq);

void
sendLApicIpi(uint32_t dest, uint32_t value);

__NAMESPACE_END