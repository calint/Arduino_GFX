/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_DataBus.h"

Arduino_DataBus::Arduino_DataBus() {}

void Arduino_DataBus::writeC8D8(uint8_t c, uint8_t d)
{
  writeCommand(c);
  write(d);
}

void Arduino_DataBus::writeC8D16(uint8_t c, uint16_t d)
{
  writeCommand(c);
  write16(d);
}

void Arduino_DataBus::writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2)
{
  writeCommand(c);
  write16(d1);
  write16(d2);
}

void Arduino_DataBus::sendCommand(uint8_t c)
{
  beginWrite();
  writeCommand(c);
  endWrite();
}

void Arduino_DataBus::sendCommand16(uint16_t c)
{
  beginWrite();
  writeCommand16(c);
  endWrite();
}

void Arduino_DataBus::sendData(uint8_t d)
{
  beginWrite();
  write(d);
  endWrite();
}

void Arduino_DataBus::sendData16(uint16_t d)
{
  beginWrite();
  write16(d);
  endWrite();
}

void Arduino_DataBus::batchOperation(spi_operation_t batch[], uint8_t len)
{
  for (uint8_t i = 0; i < len; ++i)
  {
    switch (batch[i].type)
    {
    case BEGIN_WRITE:
      beginWrite();
      break;
    case WRITE_COMMAND_8:
      writeCommand(batch[i].value);
      break;
    case WRITE_COMMAND_16:
      writeCommand16(batch[i].value);
      break;
    case WRITE_DATA_8:
      write(batch[i].value);
      break;
    case WRITE_DATA_16:
      write16(batch[i].value);
      break;
    case END_WRITE:
      endWrite();
      break;
    case DELAY:
      delay(batch[i].value);
      break;
    case SEND_COMMAND_8:
      sendCommand(batch[i].value);
      break;
    case SEND_COMMAND_16:
      sendCommand16(batch[i].value);
      break;
    case SEND_DATA_8:
      sendData(batch[i].value);
      break;
    case SEND_DATA_16:
      sendData16(batch[i].value);
      break;
    }
  }
}

#if !defined(LITTLE_FOOT_PRINT)
void Arduino_DataBus::writeIndexedPixels(uint8_t *data, uint16_t *idx, uint32_t len)
{
  while (len--)
  {
    write16(idx[*(data++)]);
  }
}

void Arduino_DataBus::writeIndexedPixelsDouble(uint8_t *data, uint16_t *idx, uint32_t len)
{
  uint8_t *d = data;
  uint16_t p;
  uint8_t hi, lo;
  while (len--)
  {
    p = idx[*(d++)];
    hi = p >> 8;
    lo = p;
    write(hi);
    write(lo);
    write(hi);
    write(lo);
  }
}
#endif // !defined(LITTLE_FOOT_PRINT)
