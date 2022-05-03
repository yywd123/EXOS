void EXOSAPI printk(uint8_t LOGTYPE, uint8_t *message)
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
      }
      break;
    case LOG_CRIT:
      {
        WriteSerialStr(COM1, "[ CRIT ] ");
        WriteSerialStr(COM1, message);
      }
      break;
    case LOG_ERROR:
      {
        WriteSerialStr(COM1, "[ ERROR ] ");
        WriteSerialStr(COM1, message);  

      }
      break;
    case LOG_WARN:
      {
        WriteSerialStr(COM1, "[ WARN ] ");
        WriteSerialStr(COM1, message);
      }
      break;
    case LOG_INFO:
      {
        WriteSerialStr(COM1, "[ INFO ] ");
        WriteSerialStr(COM1, message);
      }
      break;
    case LOG_DEBUG:
      {
        WriteSerialStr(COM1, "[ DEBUG ] ");
        WriteSerialStr(COM1, message);
      }
      break;
  }
}
