#include <exos/acpi.hpp>
#include <exos/logger.hpp>

USE(EXOS::Utils);

__NAMESPACE_DECL(Drivers::Acpi)

static uint8_t acpiRevision = 0;
static void *rsdtOrXsdt = nullptr;	//	acpiRevision=0时 为rsdt 其余为xsdt
static uint64_t acpiTableCount = 0;

static inline bool __INIT
acpiSignatureCompare(uint32_t signature, const char *s) {
	return signature == *(uint32_t *)s;
}

static inline void __INIT
showTableSignature(uint32_t signature) {
	const char buf[5] = {0};
	*((uint32_t *)&buf) = signature;
	Logger::log(Logger::INFO, "Found table @", buf);
}

static void *
__acpiMatchTable(const char *name, index_t index) {
	void *table = nullptr;
	if(acpiRevision == 0) {
		table = (void *)(uintptr_t)((Rsdt *)rsdtOrXsdt)->entries[index];
	} else {
		table = (void *)((Xsdt *)rsdtOrXsdt)->entries[index];
	}

	return __builtin_memcmp(table, name, 4) == 0 ? table : nullptr;
}

void *
__getTable(const char *name) {
	void *table = nullptr;
	__iter(acpiTableCount) {
		if((table = __acpiMatchTable(name, i)) != nullptr) return table;
	}

	return nullptr;
}

static void __INIT
parseTable(TableHeader *table) {
	showTableSignature(table->signature);
}

void __INIT
initialize(Rsdp *rsdp) {
	acpiRevision = rsdp->revision;
	if(rsdp->revision == 0) {
		Logger::log(Logger::INFO, "ACPI 1.0 detected");
		Rsdt *rsdt = (Rsdt *)(uintptr_t)rsdp->rsdtAddress;
		rsdtOrXsdt = (void *)rsdt;
		acpiTableCount = (rsdt->header.length - sizeof(TableHeader)) / sizeof(uint32_t);
		__iter(acpiTableCount) parseTable((TableHeader *)(uintptr_t)((Rsdt *)rsdtOrXsdt)->entries[i]);
	} else {
		Logger::log(Logger::INFO, "ACPI 2.0 or higher detected, revision is 0x@", rsdp->revision);
		Xsdt *xsdt = (Xsdt *)rsdp->xsdtAddress;
		rsdtOrXsdt = (void *)xsdt;
		acpiTableCount = (xsdt->header.length - sizeof(TableHeader)) / sizeof(uintptr_t);
		__iter(acpiTableCount) parseTable((TableHeader *)((Xsdt *)rsdtOrXsdt)->entries[i]);
	}
}

__NAMESPACE_END