#pragma once
#include <stdint.h>
#include <Arduino.h>
#include "../core/XSeg_Types.h"
#include "../core/XSeg_Math.h"
#include "../core/XSeg_Base.h"
#include "../core/XSeg_Buffer.h"
#include "../fonts/XSeg_Font.h"
#include "../platform/XSeg_Platform.h"

// ─────────────────────────────────────────
//  XSeg_HT16K33.h
//  Driver for HT16K33 true I2C LED driver
//  Used in Adafruit 7segment backpacks
//  and many other I2C segment modules
//
//  Features:
//  - true I2C, up to 4 devices on same bus
//  - 1-8 digits support
//  - brightness control (0-15)
//  - hardware blink (0.5, 1, 2 Hz)
//  - colon support
//  - row/column raw access
//  - non blocking
// ─────────────────────────────────────────

// ─────────────────────────────────────────
//  HT16K33 I2C Commands
// ─────────────────────────────────────────

#define HT16K33_ADDR_BASE       0x70    // default I2C address
                                        // up to 0x77 via A0-A2 pins

#define HT16K33_CMD_SYSTEM_ON   0x21    // oscillator on
#define HT16K33_CMD_SYSTEM_OFF  0x20    // oscillator off
#define HT16K33_CMD_DISPLAY_ON  0x81    // display on no blink
#define HT16K33_CMD_DISPLAY_OFF 0x80    // display off
#define HT16K33_CMD_BRIGHTNESS  0xE0    // brightness base (OR with 0-15)

// blink rates
#define HT16K33_BLINK_OFF       0x81    // display on no blink
#define HT16K33_BLINK_2HZ       0x83    // 2 Hz blink
#define HT16K33_BLINK_1HZ       0x85    // 1 Hz blink
#define HT16K33_BLINK_HALF_HZ   0x87    // 0.5 Hz blink

// colon register position
#define HT16K33_COLON_POS       0x04    // colon is at register 4


class XSeg_HT16K33 : public XSeg_Base {
public:

    // ─────────────────────────────────────
    //  Constructor
    //  addr    — I2C address (0x70-0x77)
    //  digits  — number of digits
    //  sda     — SDA pin (-1 = default)
    //  scl     — SCL pin (-1 = default)
    // ─────────────────────────────────────

    XSeg_HT16K33(uint8_t addr   = HT16K33_ADDR_BASE,
                 uint8_t digits = 4,
                 int8_t  sda    = -1,
                 int8_t  scl    = -1)
        : XSeg_Base(XSeg_Config{
            XSEG_TYPE_8,
            XSEG_COMMON_CATHODE,
            XSEG_ALIGN_RIGHT,
            XSEG_PAD_SPACE,
            digits,
            15                      // max brightness default
          })
        , _addr(addr)
        , _sda(sda)
        , _scl(scl)
        , _colonOn(false)
        , _hwBlink(HT16K33_BLINK_OFF)
        , _buffer(digits, XSEG_TYPE_8)
    {}


    // ─────────────────────────────────────
    //  Lifecycle
    // ─────────────────────────────────────

    void begin() override {
        _initI2C();

        // turn on oscillator
        _sendCmd(HT16K33_CMD_SYSTEM_ON);

        // display on no blink
        _sendCmd(HT16K33_CMD_DISPLAY_ON);

        // set brightness
        _sendCmd(HT16K33_CMD_BRIGHTNESS | _brightness);

        clear();
    }

    void clear() override {
        _buffer.clear();
        _flush();
    }

    void update() override {
        // hardware blink is handled by chip
        // so we only handle software blink here
        if (_hwBlink == HT16K33_BLINK_OFF) {
            if (!_buffer.blinkTick(_blinkMode)) {
                _sendBlank();
                return;
            }
        }

        // scroll handling
        if (_scrollEnabled) {
            if (_buffer.scrollTick(_scrollStr, _scrollSpeed, _scrollDir)) {
                _flush();
            }
            return;
        }

        // normal push
        if (_buffer.isDirty()) {
            _flush();
            _buffer.clearDirty();
        }
    }


    // ─────────────────────────────────────
    //  Show
    // ─────────────────────────────────────

    void show(int32_t n) override {
        _scrollEnabled = false;
        _buffer.writeInt(n, _align, _padding);
    }

    void show(uint32_t n) override {
        _scrollEnabled = false;
        _buffer.writeInt(n, _align, _padding);
    }

    void show(int64_t n) override {
        _scrollEnabled = false;
        _buffer.writeInt(n, _align, _padding);
    }

    void show(uint64_t n) override {
        _scrollEnabled = false;
        _buffer.writeInt(n, _align, _padding);
    }

    void show(float n, uint8_t decimals) override {
        _scrollEnabled = false;
        _buffer.writeFloat(n, decimals, _align);
    }

    void show(double n, uint8_t decimals) override {
        _scrollEnabled = false;
        _buffer.writeFloat((float)n, decimals, _align);
    }

    void show(const char* str) override {
        _scrollEnabled = false;
        _buffer.writeString(str, _align);
    }


    // ─────────────────────────────────────
    //  Raw Control
    // ─────────────────────────────────────

    void setDigit(uint8_t pos, uint16_t mask) override {
        _buffer.writeRaw(pos, mask);
    }

    void setAll(uint16_t* masks) override {
        _buffer.writeRawAll(masks);
    }


    // ─────────────────────────────────────
    //  Brightness
    //  HT16K33 supports 0-15
    // ─────────────────────────────────────

    void setBrightness(uint8_t level) override {
        _brightness = XSeg_Math::clamp(level, 0, 15);
        _sendCmd(HT16K33_CMD_BRIGHTNESS | _brightness);
    }


    // ─────────────────────────────────────
    //  Hardware Blink
    //  blink is handled by chip itself
    //  no CPU needed
    // ─────────────────────────────────────

    void setHardwareBlink(uint8_t rate) {
        _hwBlink = rate;
        _sendCmd(rate);
    }

    void setHardwareBlinkOff() {
        _hwBlink = HT16K33_BLINK_OFF;
        _sendCmd(HT16K33_BLINK_OFF);
    }

    void setHardwareBlink2Hz() {
        setHardwareBlink(HT16K33_BLINK_2HZ);
    }

    void setHardwareBlink1Hz() {
        setHardwareBlink(HT16K33_BLINK_1HZ);
    }

    void setHardwareBlinkHalfHz() {
        setHardwareBlink(HT16K33_BLINK_HALF_HZ);
    }


    // ─────────────────────────────────────
    //  Colon
    // ─────────────────────────────────────

    void setColon(bool on) {
        _colonOn = on;
        _flushColon();
    }

    bool getColon() const {
        return _colonOn;
    }


    // ─────────────────────────────────────
    //  Power
    // ─────────────────────────────────────

    void powerOn() {
        _sendCmd(HT16K33_CMD_SYSTEM_ON);
        _sendCmd(HT16K33_CMD_DISPLAY_ON);
    }

    void powerOff() {
        _sendCmd(HT16K33_CMD_DISPLAY_OFF);
        _sendCmd(HT16K33_CMD_SYSTEM_OFF);
    }


    // ─────────────────────────────────────
    //  Scroll
    // ─────────────────────────────────────

    void scroll(const char* str,
                uint16_t speedMs   = 300,
                XSeg_ScrollDir dir = XSEG_SCROLL_LEFT) {
        _scrollStr     = str;
        _scrollSpeed   = speedMs;
        _scrollDir     = dir;
        _scrollEnabled = true;
    }


private:

    // ─────────────────────────────────────
    //  Private State
    // ─────────────────────────────────────

    uint8_t      _addr;
    int8_t       _sda;
    int8_t       _scl;
    bool         _colonOn;
    uint8_t      _hwBlink;
    XSeg_Buffer  _buffer;
    const char*  _scrollStr = nullptr;


    // ─────────────────────────────────────
    //  Flush Buffer to Hardware
    // ─────────────────────────────────────

    void _flush() {
        _beginI2C();
        _writeI2C(0x00);            // start at register 0

        for (uint8_t i = 0; i < _numDigits; i++) {
            uint16_t mask = _buffer.getDigit(i);
            _writeI2C((uint8_t)(mask & 0xFF));          // low byte
            _writeI2C((uint8_t)((mask >> 8) & 0xFF));   // high byte
        }

        // colon register
        _writeI2C(_colonOn ? 0xFF : 0x00);

        _endI2C();
    }

    void _sendBlank() {
        _beginI2C();
        _writeI2C(0x00);
        for (uint8_t i = 0; i < _numDigits; i++) {
            _writeI2C(0x00);
            _writeI2C(0x00);
        }
        _writeI2C(0x00);
        _endI2C();
    }

    void _flushColon() {
        _beginI2C();
        _writeI2C(HT16K33_COLON_POS);
        _writeI2C(_colonOn ? 0xFF : 0x00);
        _endI2C();
    }


    // ─────────────────────────────────────
    //  I2C Helpers
    //  platform aware
    // ─────────────────────────────────────

    void _initI2C() {
        #if defined(ARDUINO)
            if (_sda != -1 && _scl != -1) {
                Wire.begin(_sda, _scl);     // custom pins (ESP32)
            } else {
                Wire.begin();               // default pins
            }
            Wire.setClock(400000);          // 400kHz fast mode
        #endif
    }

    void _sendCmd(uint8_t cmd) {
        _beginI2C();
        _writeI2C(cmd);
        _endI2C();
    }

    void _beginI2C() {
        #if defined(ARDUINO)
            Wire.beginTransmission(_addr);
        #endif
    }

    void _writeI2C(uint8_t data) {
        #if defined(ARDUINO)
            Wire.write(data);
        #endif
    }

    void _endI2C() {
        #if defined(ARDUINO)
            Wire.endTransmission();
        #endif
    }
};
