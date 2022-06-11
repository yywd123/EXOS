#include <EXOS.h>
#include <multiboot2.h>
#include <stdio.h>

#include <display/print.h>
//#include <dev/media/disk/disk.h>
//#include <dev/keyboard/keyboard.h>

//  defines
/**********ERRORS**********/
//  Debug
#define EXPECTION_DEBUG 0x00

//  Init
#define EXPECTION_INIT_FAILURE 0x01
#define INIT_FAILED_NO_MMAP 0x00000001

//  Kernel
#define EXPECTION_KERNEL_FAULT 0x02

//  Hardware
#define EXPECTION_HARDWARE 0x01

//  Other
#define EXPECTION_UNDEFINED 0xff

/**********STRUCTS**********/
// Video

typedef struct
{
  uint8_t bpp;
  uint32_t pitch;
  uint32_t Scrn_width;
  uint32_t Scrn_height;
  uint32_t BackGround_Color;
  uint32_t ForeGround_Color;
  uint32_t Cursor_x;
  uint32_t Cursor_y;

  void *fb;
  uint32_t fb_addr;
} VideoInfo;

// Memory

typedef struct
{
  uint64_t Base_Addr;
  uint64_t BlockSize;
} MMAP_BLOCK;

typedef struct
{
  uint32_t MemTotal;
  uint32_t MemAvailable;
  MMAP_BLOCK block[512];
} MMAP;

typedef struct
{
  unsigned Flag : 2;
  unsigned Resevered : 6;
}__attribute__((packed)) PAGE_INFO;

// Other
typedef int64_t KRNLSTAT;

typedef struct
{
  uint8_t *CmdLine;
  uint8_t *LoaderName;
  struct multiboot_tag_module *ModInfo;
  struct multiboot_tag_mmap *MemMap;
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

void      HWIO outb(uint16_t port, uint8_t data);
void      HWIO outw(uint16_t port, uint16_t data);
void      HWIO outd(uint16_t port, uint32_t data);
uint8_t   HWIO inb(uint16_t port);
uint16_t  HWIO inw(uint16_t port);
uint32_t  HWIO ind(uint16_t port);

void      HWIO io_hlt(void);
void      HWIO io_cli(void);
void      HWIO io_sti(void);

uint8_t   HWIO InitSerialPort(uint16_t port);
int       HWIO IsSerialReceived(uint16_t port);
uint8_t   HWIO ReadSerialPort(uint16_t port);
int       HWIO IsSerialTransmitEmpty(uint16_t port);
void      HWIO WriteSerialPort(uint16_t port, uint8_t data);
void      HWIO WriteSerialStr(uint16_t port, char *str);

/********************/
//      debug/debug.h

#define LOG_EMERG 0xff
#define LOG_ALERT 0xee
#define LOG_CRIT 0xdd
#define LOG_ERROR 0xcc
#define LOG_WARN 0xbb
#define LOG_INFO 0xaa
#define LOG_DEBUG 0x99

void    EXOSAPI printk(uint8_t LOGTYPE, char *message);

/********************/
//        expection.h

void EXPECTION_HANDLER(int32_t ERRCODE, uint8_t ERRTYPE, bool DUMP);


/********************/
//           memory.c

void    EXOSAPI *memcpy(void *target, const void *source, size_t size);
void    EXOSAPI *memset(void *target, const uint8_t data, size_t size);


/********************/
//      framebuffer.c

void    EXOSAPI DrawPixel(const uint32_t x, const uint32_t y, const uint32_t color);
void    EXOSAPI DrawBlock(const uint32_t x, const uint32_t y, const uint32_t h, const uint32_t v, const uint32_t color);
void    EXOSAPI ClearScreen(const uint32_t color);
void    EXOSAPI GraphicTest(void);
