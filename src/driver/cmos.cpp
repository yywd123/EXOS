#include <exos/cmos.hpp>
#include <platform/platform.hpp>
#include <exos/acpi.hpp>
#include <exos/logger.hpp>

#define CENTURY 21	//  我是21世纪先进青年(

#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

USE(EXOS::Platform::IO);
USE(EXOS::Drivers);
USE(EXOS::Utils);

__NAMESPACE_DECL(Drivers::CMOS)

static uint8_t centuryRegister = 0;
static int8_t hourOffset = 0;
static uint8_t minuteOffset = 0;

static uint8_t
readRTC(uint8_t reg) {
	outb(CMOS_ADDRESS, reg);
	return inb(CMOS_DATA);
}

static Time
readTime() {
	Time time;

	while(readRTC(0xa) & BIT(7)) ASM("pause");	//  等待时间更新完成

	time.seconds = readRTC(0x0);
	time.minutes = readRTC(0x2);
	time.hours = readRTC(0x4);
	time.dayOfWeek = readRTC(0x6);
	time.day = readRTC(0x7);
	time.month = readRTC(0x8);
	time.year = readRTC(0x9);
	if(centuryRegister) {
		time.century = readRTC(centuryRegister);
	} else
		time.century = CENTURY - 1;	 //  冷知识:世纪数要-1(21世纪对应20xx年)

	return time;
}

static uint8_t
bcdToHex(uint8_t bcd) {
	return (bcd & 0xf) + ((bcd / 16) * 10);
}

Time
getTime() {
	Time last;
	Time current = readTime();

	do {
		last = current;
		current = readTime();
	} while(!compareTime(current, last));

	uint8_t regB = readRTC(0xb);
	if(!(regB & BIT(2))) {	//  解析BCD格式(搞不懂为什么要用这么奇怪的格式)
		current.seconds = bcdToHex(current.seconds);
		current.minutes = bcdToHex(current.minutes);
		current.hours = (current.hours & 0xf) + (((current.hours & 0x70) / 16) * 10);
		current.day = bcdToHex(current.day);
		current.month = bcdToHex(current.month);
		current.year = bcdToHex(current.year);
		if(centuryRegister) current.century = bcdToHex(current.century);
	}

	if(!(regB & 0b11)) {
		current.hours = (current.hours + 12) % 24;
	}

	current.year += current.century * 100;

	current.minutes += minuteOffset;
	current.hours = current.hours + hourOffset < 0 ? current.hours + hourOffset + 24 : (current.hours + hourOffset) % 24;

	return current;
}

void
setTimeOffset(int8_t hour, uint8_t minute) {
	hour += minute / 60;
	minute %= 60;

	if(hour > 12) hour = 12;
	if(hour < -12) hour = -12;
	if(hour == 12 || hour == -12) minute = 0;
	hourOffset = hour;
	minuteOffset = minute;
	Logger::log(Logger::INFO, "set time offset to @ hour(s) and @ minute(s)", hourOffset, (int8_t)minuteOffset);
}

void __INIT
initialize() {
	auto fadt = Acpi::getTable<Acpi::Fadt>("FACP");
	if(!fadt || !fadt->centuryRegister) {
		Logger::log(Logger::WARN, "CMOS century register is not avaliable. Century default to @.", CENTURY);
	} else
		centuryRegister = fadt->centuryRegister;
}

__NAMESPACE_END