#include "../Arduino_DataBus.h"

#if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32S3)

#ifndef _ARDUINO_ST7701_RGBPANEL_H_
#define _ARDUINO_ST7701_RGBPANEL_H_

#include "../Arduino_GFX.h"
#include "../databus/Arduino_ESP32RGBPanel.h"

#define ST7701_TFTWIDTH 480
#define ST7701_TFTHEIGHT 864

static const uint8_t st7701_type1_init_operations[] = {
    BEGIN_WRITE,
    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x10,

    WRITE_C8_D16, 0xC0, 0x3B, 0x00,
    WRITE_C8_D16, 0xC1, 0x0D, 0x02,
    WRITE_C8_D16, 0xC2, 0x31, 0x05,
    WRITE_C8_D8, 0xCD, 0x08,

    WRITE_COMMAND_8, 0xB0, // Positive Voltage Gamma Control
    WRITE_BYTES, 16,
    0x00, 0x11, 0x18, 0x0E,
    0x11, 0x06, 0x07, 0x08,
    0x07, 0x22, 0x04, 0x12,
    0x0F, 0xAA, 0x31, 0x18,

    WRITE_COMMAND_8, 0xB1, // Negative Voltage Gamma Control
    WRITE_BYTES, 16,
    0x00, 0x11, 0x19, 0x0E,
    0x12, 0x07, 0x08, 0x08,
    0x08, 0x22, 0x04, 0x11,
    0x11, 0xA9, 0x32, 0x18,

    // PAGE1
    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x11,

    WRITE_C8_D8, 0xB0, 0x60, // Vop=4.7375v
    WRITE_C8_D8, 0xB1, 0x32, // VCOM=32
    WRITE_C8_D8, 0xB2, 0x07, // VGH=15v
    WRITE_C8_D8, 0xB3, 0x80,
    WRITE_C8_D8, 0xB5, 0x49, // VGL=-10.17v
    WRITE_C8_D8, 0xB7, 0x85,
    WRITE_C8_D8, 0xB8, 0x21, // AVDD=6.6 & AVCL=-4.6
    WRITE_C8_D8, 0xC1, 0x78,
    WRITE_C8_D8, 0xC2, 0x78,

    WRITE_COMMAND_8, 0xE0,
    WRITE_BYTES, 3, 0x00, 0x1B, 0x02,

    WRITE_COMMAND_8, 0xE1,
    WRITE_BYTES, 11,
    0x08, 0xA0, 0x00, 0x00,
    0x07, 0xA0, 0x00, 0x00,
    0x00, 0x44, 0x44,

    WRITE_COMMAND_8, 0xE2,
    WRITE_BYTES, 12,
    0x11, 0x11, 0x44, 0x44,
    0xED, 0xA0, 0x00, 0x00,
    0xEC, 0xA0, 0x00, 0x00,

    WRITE_COMMAND_8, 0xE3,
    WRITE_BYTES, 4, 0x00, 0x00, 0x11, 0x11,

    WRITE_C8_D16, 0xE4, 0x44, 0x44,

    WRITE_COMMAND_8, 0xE5,
    WRITE_BYTES, 16,
    0x0A, 0xE9, 0xD8, 0xA0,
    0x0C, 0xEB, 0xD8, 0xA0,
    0x0E, 0xED, 0xD8, 0xA0,
    0x10, 0xEF, 0xD8, 0xA0,

    WRITE_COMMAND_8, 0xE6,
    WRITE_BYTES, 4, 0x00, 0x00, 0x11, 0x11,

    WRITE_C8_D16, 0xE7, 0x44, 0x44,

    WRITE_COMMAND_8, 0xE8,
    WRITE_BYTES, 16,
    0x09, 0xE8, 0xD8, 0xA0,
    0x0B, 0xEA, 0xD8, 0xA0,
    0x0D, 0xEC, 0xD8, 0xA0,
    0x0F, 0xEE, 0xD8, 0xA0,

    WRITE_COMMAND_8, 0xEB,
    WRITE_BYTES, 7,
    0x02, 0x00, 0xE4, 0xE4,
    0x88, 0x00, 0x40,

    WRITE_C8_D16, 0xEC, 0x3C, 0x00,

    WRITE_COMMAND_8, 0xED,
    WRITE_BYTES, 16,
    0xAB, 0x89, 0x76, 0x54,
    0x02, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0x20,
    0x45, 0x67, 0x98, 0xBA,

    //-----------VAP & VAN---------------
    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x13,

    WRITE_C8_D8, 0xE5, 0xE4,

    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x00,

    WRITE_C8_D8, 0x3A, 0x60, // 0x70 RGB888, 0x60 RGB666, 0x50 RGB565

    WRITE_COMMAND_8, 0x11, // Sleep Out
    END_WRITE,

    DELAY, 120,

    BEGIN_WRITE,
    WRITE_COMMAND_8, 0x29, // Display On
    END_WRITE};

static const uint8_t st7701_type2_init_operations[] = {
    BEGIN_WRITE,
    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x10,

    WRITE_C8_D16, 0xC0, 0xE9, 0x03,
    WRITE_C8_D16, 0xC1, 0x11, 0x02,
    WRITE_C8_D16, 0xC2, 0x31, 0x08,

    WRITE_COMMAND_8, 0xB0, // Positive Voltage Gamma Control
    WRITE_BYTES, 16,
    0x00, 0x0D, 0x14, 0x0D,
    0x10, 0x05, 0x02, 0x08,
    0x08, 0x1E, 0x05, 0x13,
    0x11, 0xA3, 0x29, 0x18,

    WRITE_COMMAND_8, 0xB1, // Negative Voltage Gamma Control
    WRITE_BYTES, 16,
    0x00, 0x0C, 0x14, 0x0C,
    0x10, 0x05, 0x03, 0x08,
    0x07, 0x20, 0x05, 0x13,
    0x11, 0xA4, 0x29, 0x18,

    // PAGE1
    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x11,

    WRITE_C8_D8, 0xB0, 0x6C,
    WRITE_C8_D8, 0xB1, 0x43,
    WRITE_C8_D8, 0xB2, 0x07,
    WRITE_C8_D8, 0xB3, 0x80,
    WRITE_C8_D8, 0xB5, 0x47,
    WRITE_C8_D8, 0xB7, 0x8A,
    WRITE_C8_D8, 0xB8, 0x20,
    WRITE_C8_D8, 0xC1, 0x78,
    WRITE_C8_D8, 0xC2, 0x78,

    WRITE_C8_D8, 0xD0, 0x88,

    WRITE_COMMAND_8, 0xE0,
    WRITE_BYTES, 3, 0x00, 0x00, 0x02,

    WRITE_COMMAND_8, 0xE1,
    WRITE_BYTES, 11,
    0x08, 0x00, 0x0A, 0x00,
    0x07, 0x00, 0x09, 0x00,
    0x00, 0x33, 0x33,

    WRITE_COMMAND_8, 0xE2,
    WRITE_BYTES, 12,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    WRITE_COMMAND_8, 0xE3,
    WRITE_BYTES, 4, 0x00, 0x00, 0x33, 0x33,

    WRITE_C8_D16, 0xE4, 0x44, 0x44,

    WRITE_COMMAND_8, 0xE5,
    WRITE_BYTES, 16,
    0x0E, 0x60, 0xA0, 0xA0,
    0x10, 0x60, 0xA0, 0xA0,
    0x0A, 0x60, 0xA0, 0xA0,
    0x0C, 0x60, 0xA0, 0xA0,

    WRITE_COMMAND_8, 0xE6,
    WRITE_BYTES, 4, 0x00, 0x00, 0x33, 0x33,

    WRITE_C8_D16, 0xE7, 0x44, 0x44,

    WRITE_COMMAND_8, 0xE8,
    WRITE_BYTES, 16,
    0x0D, 0x60, 0xA0, 0xA0,
    0x0F, 0x60, 0xA0, 0xA0,
    0x09, 0x60, 0xA0, 0xA0,
    0x0B, 0x60, 0xA0, 0xA0,

    WRITE_COMMAND_8, 0xEB,
    WRITE_BYTES, 7,
    0x02, 0x01, 0xE4, 0xE4,
    0x44, 0x00, 0x40,

    WRITE_C8_D16, 0xEC, 0x02, 0x01,

    WRITE_COMMAND_8, 0xED,
    WRITE_BYTES, 16,
    0xAB, 0x89, 0x76, 0x54,
    0x01, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0x10,
    0x45, 0x67, 0x98, 0xBA,

    //-----------------------------------------End GIP Setting-----------------------------------------//
    //--------------------------- Power Control Registers Initial End------------------------------//
    //-------------------------------------Bank1 Setting------------------------------------------------//
    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x00,

    WRITE_C8_D8, 0x3A, 0x77, // RGB 24bits D[23:0]

    WRITE_COMMAND_8, 0x11, // Sleep Out
    END_WRITE,

    DELAY, 100,

    BEGIN_WRITE,
    WRITE_COMMAND_8, 0x29, // Display On
    END_WRITE};

static const uint8_t st7701_type3_init_operations[] = {
    BEGIN_WRITE,
    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x10,

    WRITE_C8_D16, 0xC0, 0x3B, 0x00,
    WRITE_C8_D16, 0xC1, 0x0B, 0x02, // VBP
    WRITE_C8_D16, 0xC2, 0x00, 0x02,
    WRITE_C8_D8, 0xCC, 0x10,
    WRITE_C8_D8, 0xCD, 0x08,

    WRITE_COMMAND_8, 0xB0, // Positive Voltage Gamma Control
    WRITE_BYTES, 16,
    0x02, 0x13, 0x1B, 0x0D,
    0x10, 0x05, 0x08, 0x07,
    0x07, 0x24, 0x04, 0x11,
    0x0E, 0x2C, 0x33, 0x1D,

    WRITE_COMMAND_8, 0xB1, // Negative Voltage Gamma Control
    WRITE_BYTES, 16,
    0x05, 0x13, 0x1B, 0x0D,
    0x11, 0x05, 0x08, 0x07,
    0x07, 0x24, 0x04, 0x11,
    0x0E, 0x2C, 0x33, 0x1D,

    // PAGE1
    WRITE_COMMAND_8, 0xFF, 
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x11,

    WRITE_C8_D8, 0xB0, 0x5d, // 5d
    WRITE_C8_D8, 0xB1, 0x43, // VCOM amplitude setting
    WRITE_C8_D8, 0xB2, 0x81, // VGH Voltage setting, 12V
    WRITE_C8_D8, 0xB3, 0x80,
    WRITE_C8_D8, 0xB5, 0x43, // VGL Voltage setting, -8.3V
    WRITE_C8_D8, 0xB7, 0x85,
    WRITE_C8_D8, 0xB8, 0x20,

    WRITE_C8_D8, 0xC1, 0x78,
    WRITE_C8_D8, 0xC2, 0x78,

    WRITE_C8_D8, 0xD0, 0x88,

    WRITE_COMMAND_8, 0xE0,
    WRITE_BYTES, 3, 0x00, 0x00, 0x02,

    WRITE_COMMAND_8, 0xE1,
    WRITE_BYTES, 11,
    0x03, 0xA0, 0x00, 0x00,
    0x04, 0xA0, 0x00, 0x00,
    0x00, 0x20, 0x20,

    WRITE_COMMAND_8, 0xE2,
    WRITE_BYTES, 12,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    WRITE_COMMAND_8, 0xE3,
    WRITE_BYTES, 4, 0x00, 0x00, 0x11, 0x00,

    WRITE_C8_D16, 0xE4, 0x22, 0x00,

    WRITE_COMMAND_8, 0xE5,
    WRITE_BYTES, 16,
    0x05, 0xEC, 0xA0, 0xA0,
    0x07, 0xEE, 0xA0, 0xA0,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    WRITE_COMMAND_8, 0xE6,
    WRITE_BYTES, 4, 0x00, 0x00, 0x11, 0x00,

    WRITE_C8_D16, 0xE7, 0x22, 0x00,

    WRITE_COMMAND_8, 0xE8,
    WRITE_BYTES, 16,
    0x06, 0xED, 0xA0, 0xA0,
    0x08, 0xEF, 0xA0, 0xA0,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    WRITE_COMMAND_8, 0xEB,
    WRITE_BYTES, 7,
    0x00, 0x00, 0x40, 0x40,
    0x00, 0x00, 0x00,

    WRITE_COMMAND_8, 0xED,
    WRITE_BYTES, 16,
    0xFF, 0xFF, 0xFF, 0xBA,
    0x0A, 0xBF, 0x45, 0xFF,
    0xFF, 0x54, 0xFB, 0xA0,
    0xAB, 0xFF, 0xFF, 0xFF,

    WRITE_COMMAND_8, 0xEF,
    WRITE_BYTES, 6,
    0x10, 0x0D, 0x04, 0x08,
    0x3F, 0x1F,

    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x13,

    WRITE_C8_D8, 0xEF, 0x08,

    WRITE_COMMAND_8, 0xFF,
    WRITE_BYTES, 5, 0x77, 0x01, 0x00, 0x00, 0x00,

    WRITE_COMMAND_8, 0x11, // Sleep Out
    END_WRITE,

    DELAY, 120,

    BEGIN_WRITE,
    WRITE_COMMAND_8, 0x29,   // Display On
    WRITE_C8_D8, 0x36, 0x00, // 
    WRITE_C8_D8, 0x3A, 0x60, // 0x60 18bit   0x50 16bit
    END_WRITE};

class Arduino_ST7701_RGBPanel : public Arduino_GFX
{
public:
  Arduino_ST7701_RGBPanel(
      Arduino_ESP32RGBPanel *bus, int8_t rst = GFX_NOT_DEFINED, uint8_t r = 0,
      bool ips = false, int16_t w = ST7701_TFTWIDTH, int16_t h = ST7701_TFTHEIGHT,
      const uint8_t *init_operations = st7701_type1_init_operations,
      size_t init_operations_len = sizeof(st7701_type1_init_operations),
      bool bgr = true);

  void begin(int32_t speed = GFX_NOT_DEFINED) override;
  void writePixelPreclipped(int16_t x, int16_t y, uint16_t color) override;
  void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
  void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
  void writeFillRectPreclipped(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
  void draw16bitRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h) override;
  void draw16bitBeRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h) override;

  void setRotation(uint8_t r) override;
  void invertDisplay(bool) override;

  uint16_t *getFramebuffer();

protected:
  uint16_t *_framebuffer;
  Arduino_ESP32RGBPanel *_bus;
  int8_t _rst;
  bool _ips;
  const uint8_t *_init_operations;
  size_t _init_operations_len;
  bool _bgr;

private:
};

#endif // _ARDUINO_ST7701_RGBPANEL_H_

#endif // #if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32S3)
