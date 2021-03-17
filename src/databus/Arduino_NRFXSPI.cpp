/*
 * start rewrite from:
 * https://github.com/arduino/ArduinoCore-mbed/blob/master/libraries/SPI/SPI.cpp
 */
#ifdef ARDUINO_ARCH_NRF52840

#include "Arduino_NRFXSPI.h"

Arduino_NRFXSPI::Arduino_NRFXSPI(int8_t dc, int8_t cs /* = -1 */, int8_t sck /* = -1 */, int8_t mosi /* = -1 */, int8_t miso /* = -1 */)
    : _dc(dc), _cs(cs), _sck(sck), _mosi(mosi), _miso(miso)
{
}

void Arduino_NRFXSPI::begin(int32_t speed, int8_t dataMode)
{
  _speed = speed ? speed : SPI_DEFAULT_FREQ;
  _dataMode = dataMode;

  // init pin mask
  uint32_t pin = digitalPinToPinName((pin_size_t)_dc);
  NRF_GPIO_Type *reg = nrf_gpio_pin_port_decode(&pin);
  nrf_gpio_cfg_output(pin);
  _dcPortSet = &reg->OUTSET;
  _dcPortClr = &reg->OUTCLR;
  _dcPinMask = 1UL << pin;
  if (_cs >= 0)
  {
    pin = digitalPinToPinName((pin_size_t)_cs);
    reg = nrf_gpio_pin_port_decode(&pin);
    nrf_gpio_cfg_output(pin);
    _csPortSet = &reg->OUTSET;
    _csPortClr = &reg->OUTCLR;
    _csPinMask = 1UL << pin;
  }

  // init SPI pins
  _nrfxSpiConfig.sck_pin = digitalPinToPinName((pin_size_t)_sck);
  _nrfxSpiConfig.mosi_pin = digitalPinToPinName((pin_size_t)_mosi);
  if (_miso > 0)
  {
    _nrfxSpiConfig.miso_pin = digitalPinToPinName((pin_size_t)_miso);
  }
  else
  {
    _nrfxSpiConfig.miso_pin = NRFX_SPI_PIN_NOT_USED;
  }

  // init speed
  if (_speed >= 8000000)
  {
    _nrfxSpiConfig.frequency = NRF_SPI_FREQ_8M;
  }
  else if (_speed >= 4000000)
  {
    _nrfxSpiConfig.frequency = NRF_SPI_FREQ_4M;
  }
  else if (_speed >= 2000000)
  {
    _nrfxSpiConfig.frequency = NRF_SPI_FREQ_2M;
  }
  else if (_speed >= 1000000)
  {
    _nrfxSpiConfig.frequency = NRF_SPI_FREQ_1M;
  }
  else if (_speed >= 500000)
  {
    _nrfxSpiConfig.frequency = NRF_SPI_FREQ_500K;
  }
  else if (_speed >= 250000)
  {
    _nrfxSpiConfig.frequency = NRF_SPI_FREQ_250K;
  }
  else if (_speed >= 125000)
  {
    _nrfxSpiConfig.frequency = NRF_SPI_FREQ_125K;
  }

  // init data mode
  if (_dataMode < 0)
  {
    _dataMode = SPI_MODE0;
    _nrfxSpiConfig.mode = NRF_SPI_MODE_0;
  }
  else if (_dataMode == SPI_MODE1)
  {
    _nrfxSpiConfig.mode = NRF_SPI_MODE_1;
  }
  else if (_dataMode == SPI_MODE2)
  {
    _nrfxSpiConfig.mode = NRF_SPI_MODE_2;
  }
  else if (_dataMode == SPI_MODE3)
  {
    _nrfxSpiConfig.mode = NRF_SPI_MODE_3;
  }

  // init SPI
  nrfx_spi_init(&_nrfxSpi, &_nrfxSpiConfig, NULL, NULL);
}

void Arduino_NRFXSPI::beginWrite()
{
  DC_HIGH();

  CS_LOW();
}

void Arduino_NRFXSPI::writeCommand(uint8_t c)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();
}

void Arduino_NRFXSPI::writeCommand16(uint16_t c)
{
  DC_LOW();

  WRITE16(c);

  DC_HIGH();
}

void Arduino_NRFXSPI::write(uint8_t d)
{
  WRITE(d);
}

void Arduino_NRFXSPI::write16(uint16_t d)
{
  WRITE16(d);
}

void Arduino_NRFXSPI::write32(uint32_t d)
{
  WRITE32(d);
}

void Arduino_NRFXSPI::writeC8D8(uint8_t c, uint8_t d)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();

  WRITE(d);
}

void Arduino_NRFXSPI::writeC8D16(uint8_t c, uint16_t d)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();

  WRITE16(d);
}

void Arduino_NRFXSPI::writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2)
{
  DC_LOW();

  const nrfx_spi_xfer_desc_t xfer_desc1 = NRFX_SPI_SINGLE_XFER(&c, 1, NULL, 0);
  nrfx_spi_xfer(&_nrfxSpi, &xfer_desc1, 0);

  DC_HIGH();

  uint32_t d;
  MSB_32_16_16_SET(d, d1, d2);
  const nrfx_spi_xfer_desc_t xfer_desc2 = NRFX_SPI_SINGLE_XFER(&d, 4, NULL, 0);
  nrfx_spi_xfer(&_nrfxSpi, &xfer_desc2, 0);
}

void Arduino_NRFXSPI::endWrite()
{
  CS_HIGH();
}

void Arduino_NRFXSPI::sendCommand(uint8_t c)
{
  CS_LOW();

  DC_LOW();

  WRITE(c);

  DC_HIGH();

  CS_HIGH();
}

void Arduino_NRFXSPI::sendCommand16(uint16_t c)
{
  CS_LOW();

  DC_LOW();

  WRITE16(c);

  DC_HIGH();

  CS_HIGH();
}

void Arduino_NRFXSPI::sendData(uint8_t d)
{
  CS_LOW();

  WRITE(d);

  CS_HIGH();
}

void Arduino_NRFXSPI::sendData16(uint16_t d)
{
  CS_LOW();

  WRITE16(d);

  CS_HIGH();
}

void Arduino_NRFXSPI::sendData32(uint32_t d)
{
  CS_LOW();

  WRITE32(d);

  CS_HIGH();
}

void Arduino_NRFXSPI::writeRepeat(uint16_t p, uint32_t len)
{
  MSB_16_SET(p, p);
  uint32_t bufLen = (len < SPI_MAX_PIXELS_AT_ONCE) ? len : SPI_MAX_PIXELS_AT_ONCE;
  uint32_t xferLen;
  for (uint32_t i = 0; i < bufLen; i++)
  {
    buffer16[i] = p;
  }

  while (len)
  {
    xferLen = (bufLen < len) ? bufLen : len;
    WRITEBUF(buffer, xferLen * 2);
    len -= xferLen;
  }
}

void Arduino_NRFXSPI::writeBytes(uint8_t *data, uint32_t len)
{
  WRITEBUF(data, len);
}

void Arduino_NRFXSPI::writePixels(uint16_t *data, uint32_t len)
{
  uint32_t xferLen;
  uint8_t *p;
  union
  {
    uint16_t val;
    struct
    {
      uint8_t lsb;
      uint8_t msb;
    };
  } t;
  while (len)
  {
    xferLen = (len < SPI_MAX_PIXELS_AT_ONCE) ? len : SPI_MAX_PIXELS_AT_ONCE;
    p = buffer;
    for (uint32_t i = 0; i < xferLen; i++)
    {
      t.val = *data++;
      *p++ = t.msb;
      *p++ = t.lsb;
    }
    xferLen += xferLen;
    WRITEBUF(buffer, xferLen);
    len -= xferLen;
  }
}

/**
 * @param data uint8_t *
 * @param len uint8_t
 * @param repeat uint32_t
 */
void Arduino_NRFXSPI::writePattern(uint8_t *data, uint8_t len, uint32_t repeat)
{
  while (repeat--)
  {
    for (uint8_t i = 0; i < len; i++)
    {
      write(data[i]);
    }
  }
}

INLINE void Arduino_NRFXSPI::WRITE(uint8_t d)
{
  const nrfx_spi_xfer_desc_t xfer_desc = NRFX_SPI_SINGLE_XFER(&d, 1, NULL, 0);
  nrfx_spi_xfer(&_nrfxSpi, &xfer_desc, 0);
}

INLINE void Arduino_NRFXSPI::WRITE16(uint16_t d)
{
  MSB_16_SET(d, d);
  const nrfx_spi_xfer_desc_t xfer_desc = NRFX_SPI_SINGLE_XFER(&d, 2, NULL, 0);
  nrfx_spi_xfer(&_nrfxSpi, &xfer_desc, 0);
}

INLINE void Arduino_NRFXSPI::WRITE32(uint32_t d)
{
  MSB_32_SET(d, d);
  const nrfx_spi_xfer_desc_t xfer_desc = NRFX_SPI_SINGLE_XFER(&d, 4, NULL, 0);
  nrfx_spi_xfer(&_nrfxSpi, &xfer_desc, 0);
}

INLINE void Arduino_NRFXSPI::WRITEBUF(uint8_t *buf, size_t count)
{
  const nrfx_spi_xfer_desc_t xfer_desc = NRFX_SPI_SINGLE_XFER(buf, count, NULL, 0);
  nrfx_spi_xfer(&_nrfxSpi, &xfer_desc, 0);
}

/******** low level bit twiddling **********/

INLINE void Arduino_NRFXSPI::CS_HIGH(void)
{
  if (_cs >= 0)
  {
    *_csPortSet = _csPinMask;
  }
}

INLINE void Arduino_NRFXSPI::CS_LOW(void)
{
  if (_cs >= 0)
  {
    *_csPortClr = _csPinMask;
  }
}

INLINE void Arduino_NRFXSPI::DC_HIGH(void)
{
  *_dcPortSet = _dcPinMask;
}

INLINE void Arduino_NRFXSPI::DC_LOW(void)
{
  *_dcPortClr = _dcPinMask;
}

#endif // #ifdef ARDUINO_ARCH_NRF52840
