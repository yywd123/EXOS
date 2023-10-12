#include <vector>

#pragma once

#define DRIVER_MAGIC (0x01145140)

__NAMESPACE_DECL(Drivers)

typedef enum class DriverType : uint32_t {
	PlatformInit,
	Init,
	PostInit,
	Pci
} DriverType;

typedef struct {
	uint32_t magic;
	DriverType type;
	const char *name;
	Fn<bool, void *> initialize;
	Fn<bool> enable;
	Fn<bool> disable;
	Fn<bool> uninstall;
	uint64_t pad0;
	uint64_t pad1;
} Driver;

#define DECL_DRIVER(name) Drivers::Driver __attr(section(".builtin_drvlist")) name

void
initializeManager();

void
getDrivers(DriverType type, std::vector<Driver *> *out);

__NAMESPACE_END