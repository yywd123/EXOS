#include <EXOS.h>
#include <multiboot2.h>
#include <stdio.h>

#include <display/display.h>
#include <display/print.h>
#include <dev/memory/memory.h>
//#include <dev/media/disk/disk.h>
//#include <dev/keyboard/keyboard.h>

// structs
typedef struct
{
  uint8_t *CmdLine;
  uint8_t *LoaderName;
  struct multiboot_tag_module *ModInfo;
  struct multiboot_tag_mmap *MemMap;
  uint32_t mem_lower, mem_upper;
} BootInfo;

// Kernel Functions
/********************/
//            io/io.h

#define iret __asm__ __volatile__("iret")

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8
#define COM5 0x5F8
#define COM6 0x4F8
#define COM7 0x5E8
#define COM8 0x4E8

void EXOSAPI outb(uint16_t port, uint8_t data);
void EXOSAPI outw(uint16_t port, uint16_t data);
void EXOSAPI outd(uint16_t port, uint32_t data);
uint8_t EXOSAPI inb(uint16_t port);
uint16_t EXOSAPI inw(uint16_t port);
uint32_t EXOSAPI ind(uint16_t port);

void EXOSAPI io_hlt(void);

uint8_t EXOSAPI InitSerialPort(uint16_t port);
int EXOSAPI IsSerialReceived(uint16_t port);
uint8_t EXOSAPI ReadSerialPort(uint16_t port);
int EXOSAPI IsSerialTransmitEmpty(uint16_t port);
void EXOSAPI WriteSerialPort(uint16_t port, uint8_t data);
void EXOSAPI WriteSerialStr(uint16_t port, char *str);

/********************/
//      debug/debug.h

#define LOG_EMERG 0xff
#define LOG_ALERT 0xee
#define LOG_CRIT 0xdd
#define LOG_ERROR 0xcc
#define LOG_WARN 0xbb
#define LOG_INFO 0xaa
#define LOG_DEBUG 0x99

void EXOSAPI printk(uint8_t LOGTYPE, char *message);

/********************/
//        expection.h

void EXOSAPI EXPECTION_HANDLER(int32_t ERRCODE, uint8_t ERRTYPE, bool DUMP);
