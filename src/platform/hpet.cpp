#include <exos/hpet.hpp>
#include <exos/acpi.hpp>
#include <exos/panic.hpp>
#include <exos/logger.hpp>

USE(EXOS::Utils);

typedef struct {
	uint64_t configurationAndCapability;
	uint64_t comparatorValue;
	uint64_t fsbInterruptRoute;
	uint64_t unused;
} __packed HpetTimer;

typedef struct {
	uint64_t generalCapabilities;
	uint64_t reserved0;
	uint64_t generalConfiguration;
	uint64_t reserved1;
	uint64_t generalIntrruptStatus;
	uint8_t reserved3[0xc8];
	uint64_t mainCounterValue;
	uint64_t reserved4;
	HpetTimer timers[0];
} __packed HpetInfo;

static HpetInfo *hpetInfo = nullptr;
static uint64_t hpetPeriod = 0;

__NAMESPACE_DECL(Drivers::Hpet)

void __INIT
initialize() {
	auto hpet = Acpi::getTable<Acpi::Hpet>("HPET");
	if(!hpet) {
		panic("can not found acpi hpet table");
	}
	hpetInfo = (HpetInfo *)hpet->hpetAddress.address;
	Logger::log(Logger::DEBUG, "hpetInfo @", hpetInfo);

	uint32_t counterClockPeriod = hpetInfo->generalCapabilities >> 32;
	hpetPeriod = counterClockPeriod / 1000000;
	Logger::log(Logger::DEBUG, "hpet period: 0x@", hpetPeriod);

	hpetInfo->generalConfiguration |= BIT(0);	 //  启用hpet

	Logger::log(Logger::DEBUG, "hpet successfully enabled");
}

uint64_t
nanoTime() {
	return hpetInfo->mainCounterValue * hpetPeriod;
}

uint64_t
rawTime() {
	return hpetInfo->mainCounterValue;
}

uint64_t
getPeriod() {
	return hpetPeriod;
}

void
sleep(uint64_t nano) {
	uint64_t targetTime = nanoTime() + nano;
	while(nanoTime() < targetTime) ASM("pause");
}

__NAMESPACE_END