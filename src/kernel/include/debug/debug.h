/*  Debug Library for EXOS
 *  Copyright (C) 2020-2022 yywd_123
 *  Author:yywd_123
 *  Date:2022-5-17
*/

#define LOG_EMERG 0xff
#define LOG_ALERT 0xee
#define LOG_CRIT 0xdd
#define LOG_ERROR 0xcc
#define LOG_WARN 0xbb
#define LOG_INFO 0xaa
#define LOG_DEBUG 0x99

void EXOSAPI printk(uint8_t LOGTYPE, char *message)
{
  switch(LOGTYPE)
  {
    case LOG_EMERG:
      {
        // call expection handler
      }
      break;
    case LOG_ALERT:
      {
        WriteSerialStr(COM1, "[ ALERT ] ");
        WriteSerialStr(COM1, message);
        WriteSerialPort(COM1, '\n');
      }
      break;
    case LOG_CRIT:
      {
        WriteSerialStr(COM1, "[ CRIT ] ");
        WriteSerialStr(COM1, message); 
        WriteSerialPort(COM1, '\n');
      }
      break;
    case LOG_ERROR:
      {
        WriteSerialStr(COM1, "[ ERROR ] ");
        WriteSerialStr(COM1, message);  
        WriteSerialPort(COM1, '\n');
      }
      break;
    case LOG_WARN:
      {
        WriteSerialStr(COM1, "[ WARN ] ");
        WriteSerialStr(COM1, message);
        WriteSerialPort(COM1, '\n');
      }
      break;
    case LOG_INFO:
      {
        WriteSerialStr(COM1, "[ INFO ] ");
        WriteSerialStr(COM1, message);
        WriteSerialPort(COM1, '\n');
      }
      break;
    case LOG_DEBUG:
      {
        WriteSerialStr(COM1, "[ DEBUG ] ");
        WriteSerialStr(COM1, message);
        WriteSerialPort(COM1, '\n');
      }
      break;
  }
}
