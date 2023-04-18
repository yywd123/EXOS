#include <utils>
#include <acpi/acpi.h>
#include <driver/IO>
#include <lib/Logger>

using namespace EXOS;
using namespace EXOS::Utils;

static inline bool acpiSignatureCompare(uint32_t signature, const char *s) {
	return signature == *(uint32_t*)s;
}

AcpiFadt *fadt;

static void acpiEnable(AcpiFadt *p) {
  fadt = p;

  if (checkFlag(IO::inw(fadt->pm1aControlBlock), BIT(0))) return;

  if (fadt->smiCommandPort && fadt->acpiEnable) {
    IO::outb(fadt->smiCommandPort, fadt->acpiEnable);
    while (!checkFlag(IO::inw(fadt->pm1aControlBlock), BIT(0)));
    if (fadt->pm1bControlBlock) 
      while (!checkFlag(IO::inw(fadt->pm1bControlBlock), BIT(0)));
  }
}


static void parseAcpiTable(AcpiHeader *header) {
  if (acpiSignatureCompare(header->signature, "FACP")) {
    acpiEnable((AcpiFadt*)header);
    Logger::log(Logger::INFO, "ACPI successfully enabled");
  }
}

void acpiInit(void *rsdptr) {
  Logger::beginSection("acpiInit");
  AcpiRsdp *rsdp = (AcpiRsdp*)rsdptr;

  if (rsdp->revision == 0) {
    Logger::log(Logger::INFO, "ACPI 1.0 detected");
    AcpiRsdt *rsdt = (AcpiRsdt*)(uintptr_t)rsdp->rsdtAddress;
    uint32_t entryCount = (rsdt->header.length - sizeof(AcpiHeader)) / sizeof(uint32_t);
    iter(entryCount) parseAcpiTable((AcpiHeader*)(uintptr_t)rsdt->entries[i]);
  } else {
    Logger::log(Logger::INFO, "ACPI 2.0 or higher detected, revision is 0x@", rsdp->revision);
    AcpiXsdt *xsdt = (AcpiXsdt*)rsdp->xsdtAddress;
    uint64_t entryCount = (xsdt->header.length - sizeof(AcpiHeader)) / sizeof(uintptr_t);
    iter(entryCount) parseAcpiTable((AcpiHeader*)xsdt->entries[i]);
  }

  Logger::log(Logger::INFO, "end init");
  Logger::endSection();
}