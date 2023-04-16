#include <utils>
#include <acpi/acpi.h>
#include <lib/SerialOutputStream>
#include <lib/PrintWriter>
#include <driver/IO>

using namespace EXOS;

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


static void parseAcpiTable(AcpiHeader *header, Utils::PrintWriter logger) {
  if (acpiSignatureCompare(header->signature, "FACP")) {
    logger.println("FADT");
    acpiEnable((AcpiFadt*)header);
    logger.println("[ACPI] ACPI successfully enabled");
  }
}

void acpiInit(void *rsdptr) {
  Utils::SerialOutputStream debugStream(Driver::Serial::COM1);
  Utils::PrintWriter logger(&debugStream);
  AcpiRsdp *rsdp = (AcpiRsdp*)rsdptr;

  if (rsdp->revision == 0) {
    logger.println("[ACPI] ACPI 1.0 detected");
    AcpiRsdt *rsdt = (AcpiRsdt*)(uintptr_t)rsdp->rsdtAddress;
    uint32_t entryCount = (rsdt->header.length - sizeof(AcpiHeader)) / sizeof(uint32_t);
    iter(entryCount) parseAcpiTable((AcpiHeader*)(uintptr_t)rsdt->entries[i], logger);
  } else {
    logger.println("[ACPI] ACPI 2.0 or higher detected");
    AcpiXsdt *xsdt = (AcpiXsdt*)rsdp->xsdtAddress;
    uint64_t entryCount = (xsdt->header.length - sizeof(AcpiHeader)) / sizeof(uintptr_t);
    iter(entryCount) parseAcpiTable((AcpiHeader*)xsdt->entries[i], logger);
  }

  logger.println("[ACPI] end init");
}