#include <utils>
#include <bootloader.h>
#include <uefi.h>
#include <acpi/acpi.h>
#include <display/DisplayUtils>
#include <driver/graphics/BasicFramebufferDriver>
#include <display/ASCIIFontRender>

extern void cxxabiEarlyInit();

extern void archInit(BootConfig *conf);

using namespace EXOS;
using namespace EXOS::Driver;

void missingConfigurationTable(const char* name);

extern "C" attr(section(".text.entry"), noreturn) void kernelEntry(BootConfig *conf) {
  cxxabiEarlyInit();
  archInit(conf);
  Display::setDisplayAdapter(new Graphics::BasicFramebufferDriver(conf->graphicsInfo.fbAddress, conf->graphicsInfo.width, conf->graphicsInfo.height));

  Display::drawRect(0, 0, 2048, 2048, 0x39c5bb);

  Display::ASCIIFontRenderer renderer;
  renderer.renderString(0, 0, 0xffffff, "wow ASCIIFontRenderer works!");

  EfiConfTableEntry *entry = (EfiConfTableEntry*)conf->efiConfigurationTable;

  void *acpiRsdptr = nullptr;
  void *acpi2Rsdptr = nullptr;
  
  #define EntryMatcher(_guid, _table) if (GuidCompare(entry[i].guid, _guid)) {_table = entry[i].table;continue;}
  iter(conf->tableEntryCount) {
    EntryMatcher(__GUID({0xeb9d2d30,0x2d88,0x11d3,{0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d}}), acpiRsdptr)
    EntryMatcher(__GUID({0x8868e871,0xe4f1,0x11d3,{0xbc,0x22,0x00,0x80,0xc7,0x3c,0x88,0x81}}), acpi2Rsdptr)
  }
  #undef EntryMatcher

  if (acpi2Rsdptr) acpiInit(acpi2Rsdptr);
  else if (acpiRsdptr) acpiInit(acpiRsdptr);
  else missingConfigurationTable("ACPI RSDP");

  ASM("int3\n\tud2\n\tint3");
  while (1);
  __builtin_unreachable();
}

void missingConfigurationTable(const char* name) {
  while (1);
}