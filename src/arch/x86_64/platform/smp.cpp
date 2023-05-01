#include <utils.hpp>
#include <acpi/acpi.h>
#include <lib/Logger.hpp>

typedef struct {
  uint8_t type;
  uint8_t length;
} MadtEntry;

typedef struct {
  MadtEntry header;
  uint8_t apicProcessorID;
  uint8_t apicID;
  uint32_t flags;
} ProcessorLAPICEntry;

typedef struct {
  MadtEntry header;
  uint8_t ioapicID;
  uint8_t reserved;
  uint32_t ioapicAddress;
  uint32_t intrBase;
} IOAPICEntry;

typedef struct {
  MadtEntry header;
  uint16_t reserved;
  uintptr_t lapicAddr;
} LAPICAddressEntry;

enum {
  ProcessorLAPIC = 0,
  IOAPIC,
  IOAPICIntrSrc,
  IOAPICNMI,
  LAPICNMI,
  LAPICAddr,
  ProcessorLx2APIC = 9
};

static uint8_t *lapicIDs = nullptr;
static uint8_t coreCount = 0;
static uint32_t ioapicPtr = 0;
static uint32_t lapicPtr = 0;

using namespace EXOS::Driver;
using namespace EXOS::Utils;

static void parseMadt(AcpiMadt *madt) {
  lapicPtr = madt->lapicAddr;
parse:
  coreCount = 0;
  uint8_t *p = madt->data;
  uint32_t length = madt->header.length - sizeof(AcpiMadt);
  while (length != 0) {
    MadtEntry *entry = (MadtEntry*)p;
    p += entry->length;
    length -= entry->length;
    switch (entry->type) {
      match(ProcessorLAPIC, {
        ProcessorLAPICEntry *e = (ProcessorLAPICEntry*)entry;
        if (checkFlag(e->flags, BIT(0)) && lapicIDs) {
          lapicIDs[coreCount] = e->apicID;
          Logger::log(Logger::INFO, "new core detected, lapic id is @", e->apicID);
        }
        ++coreCount;
      });
      match(IOAPIC, {
        if (!lapicIDs) continue;
        IOAPICEntry *e = (IOAPICEntry*)entry;
        ioapicPtr = e->ioapicAddress;
      });
      match(LAPICAddr, {
        if (!lapicIDs) continue;
        LAPICAddressEntry *e = (LAPICAddressEntry*)entry;
        lapicPtr = e->lapicAddr;
      });
      default: continue;
    }
  }
  if (lapicIDs) return;

  lapicIDs = new uint8_t[coreCount];
  goto parse;
}

extern void apicInit(uint32_t ioapic, uint32_t lapic);
void smpInit(AcpiMadt *madt) {
  Logger::saveAndBeginSection("SMPInit");
  parseMadt(madt);
  Logger::log(Logger::DEBUG, "found @ core(s), ioapic address is @, lapic address is @", (short)coreCount, ioapicPtr, lapicPtr);
  // apicInit(ioapicPtr, lapicPtr);

  Logger::restoreSection();
}
