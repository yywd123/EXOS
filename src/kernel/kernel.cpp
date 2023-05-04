#include <utils.hpp>
#include <bootloader.h>
#include <uefi.h>
#include <acpi/acpi.h>
#include <display/DisplayUtils.hpp>
#include <driver/graphics/BasicFramebufferDriver.hpp>
#include <driver/graphics/FramebufferConsole.hpp>
#include <lib/Logger.hpp>
#include <lib/SimpleOutputStream.hpp>
#include <driver/Serial.hpp>
#include <lib/BmpViewer.hpp>

extern void cxxabiEarlyInit();

extern void archInit(BootConfig *conf);

using namespace EXOS;
using namespace EXOS::Driver;
using namespace EXOS::Utils;

void missingConfigurationTable(const char* name);

extern "C" attr(section(".text.entry"), noreturn) void kernelEntry(BootConfig *conf) {
  ASM("cli");
  cxxabiEarlyInit();

  Display::setDisplayAdapter(
    new Graphics::BasicFramebufferDriver(
      conf->graphicsInfo.fbAddress, 
      conf->graphicsInfo.width, 
      conf->graphicsInfo.height)
  );
  Graphics::FramebufferConsole::init(Display::getDisplayAdapter());

  Logger::loggerStream = new SimpleOutputStream([](uint8_t byte) {
    Serial::write(Serial::COM1, byte);
    Graphics::FramebufferConsole::print(byte);
  });

  BmpViewer::BmpHeader *header = (BmpViewer::BmpHeader*)0x200000;
  BmpViewer::displayBitmap(
    (conf->graphicsInfo.width - header->frameWidth) / 2, 
    (conf->graphicsInfo.height - header->frameHeight) / 2, 
    true, 
    header
  );
  
  archInit(conf);

  Logger::beginSection("parseEfiConfTable");
  EfiConfTableEntry *entry = (EfiConfTableEntry*)conf->efiConfigurationTable;

  void *acpiRsdptr = nullptr;
  void *acpi2Rsdptr = nullptr;
  
  #define EntryMatcher(_guid, _table) if (GuidCompare(entry[i].guid, _guid)) {_table = entry[i].table;continue;}
  iter(conf->tableEntryCount) {
    Logger::log(Logger::DEBUG, "current Entry: guid=@, table=@", entry[i].guid, entry[i].table);
    EntryMatcher(__GUID({0xeb9d2d30,0x2d88,0x11d3,{0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d}}), acpiRsdptr)
    EntryMatcher(__GUID({0x8868e871,0xe4f1,0x11d3,{0xbc,0x22,0x00,0x80,0xc7,0x3c,0x88,0x81}}), acpi2Rsdptr)
  }
  #undef EntryMatcher

  Logger::log(Logger::DEBUG, "acpiv1 @ acpiv2 @", acpiRsdptr, acpi2Rsdptr);

  if (acpi2Rsdptr) acpiInit(acpi2Rsdptr);
  else if (acpiRsdptr) acpiInit(acpiRsdptr);
  else missingConfigurationTable("ACPI RSDP");

  Logger::log(Logger::INFO, "init done");
  Logger::print("> ");
  ASM("sti");

  while (1);
  __builtin_unreachable();
}

void missingConfigurationTable(const char* name) {
  Logger::log(Logger::FATAL, "CAN NOT find table [@]", name);
  while (1);
}