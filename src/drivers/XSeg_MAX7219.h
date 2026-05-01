#pragma once
#include <stdint.h>
#include <Arduino.h>
#if defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_SAMD) || defined(ESP32) || defined(ESP8266)
    #include <SPI.h>
#endif
#include "../core/XSeg_Types.h"
#include "../core/XSeg_Math.h"
#include "../core/XSeg_Base.h"
#include "../core/XSeg_Buffer.h"
#include "../fonts/XSeg_Font.h"
#include "../platform/XSeg_Platform.h"

// ─────────────────────────────────────────
//  XSeg_MAX7219.h
//  Driver for MAX7219 SPI LED driver
//  Supports up to 8 digits per chip
//  Supports chaining multiple chips
//
//  Features:
//  - up to 8 digits per chip
//  - chainable (multiple chips)
//  - hardware SPI or bit bang
//  - brightness control (0-15)
//  - shutdown mode
//  - test mode
//  - no decode mode (raw segment control)
// ─────────────────────────────────────────

// ─────────────────────────────────────────
//  MAX7219 Register Addresses
// ─────────────────────────────────────────

#define MAX7219_REG_NOOP        0x00
#define MAX7219_REG_DIGIT0      0x01
#define MAX7219_REG_DIGIT1      0x02
#define MAX7219_REG_DIGIT2      0x03
#define MAX7219_REG_DIGIT3      0x04
#define MAX7219_REG_DIGIT4      0x05
#define MAX7219_REG_DIGIT5      0x06
#define MAX7219_REG_DIGIT6      0x07
#define MAX7219_REG_DIGIT7      0x08
#define MAX7219_REG_DECODEMODE  0x09
#define MAX7219_REG_INTENSITY   0x0A
#define MAX7219_REG_SCANLIMIT   0x0B
#define MAX7219_REG_SHUTDOWN    0x0C
#define MAX7219_REG_DISPLAYTEST 0x0F


class XSeg_MAX7219 : public XSeg_Base {
public:

    // ─────────────────────────────────────
    //  Constructor
    //  cs       — chip select pin
    //  digits   — number of digits (1-8 per chip)
    //  chains   — number of chained chips
    //  clk      — clock pin (-1 = use hardware SPI)
    //  din      — data pin  (-1 = use hardware SPI)
    // ─────────────────────────────────────

    XSeg_MAX7219(uint8_t cs,
                 uint8_t digits  = 8,
                 uint8_t chains  = 1,
                 int8_t  clk     = -1,
                 int8_t  din     = -1)
        : XSeg_Base(XSeg_Config{
            XSEG_TYPE_8,
            XSEG_COMMON_CATHODE,
            XSEG_ALIGN_RIGHT,
            XSEG_PAD_SPACE,
            digits,
            8                       // mid brightness default
          })
        , _cs(cs)
        , _clk(clk)
        , _din(din)
        , _chains(chains)
        , _softSPI(clk != -1 && din != -1)
        , _shutdown(false)
        , _buffer(digits, XSEG_TYPE_8)
    {}


    // ─────────────────────────────────────
    //  Lifecycle
    // ─────────────────────────────────────

    void begin() override {
        // setup pins
        XSEG_PIN_OUTPUT(_cs);
        XSEG_PIN_HIGH(_cs);

        if (_softSPI) {
            XSEG_PIN_OUTPUT(_clk);
            XSEG_PIN_OUTPUT(_din);
            XSEG_PIN_HIGH(_clk);
            XSEG_PIN_LOW(_din);
        } else {
            _initHardwareSPI();
        }

        // init all chained chips
        for (uint8_t c = 0; c < _chains; c++) {
            _sendReg(c, MAX7219_REG_SHUTDOWN,    0x01);  // normal operation
            _sendReg(c, MAX7219_REG_DISPLAYTEST, 0x00);  // test mode off
            _sendReg(c, MAX7219_REG_DECODEMODE,  0x00);  // no BCD decode
            _sendReg(c, MAX7219_REG_SCANLIMIT,   _numDigits - 1);
            _sendReg(c, MAX7219_REG_INTENSITY,   _brightness);
        }

        clear();
    }

    void clear() override {
        _buffer.clear();
        _flush();
    }

    void update() override {
        // blink handling
        if (!_buffer.blinkTick(_blinkMode)) {
            _sendBlank();
            return;
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
    //  MAX7219 supports 0-15
    // ─────────────────────────────────────

    void setBrightness(uint8_t level) override {
        _brightness = XSeg_Math::clamp(level, 0, 15);
        for (uint8_t c = 0; c < _chains; c++) {
            _sendReg(c, MAX7219_REG_INTENSITY, _brightness);
        }
    }


    // ─────────────────────────────────────
    //  Shutdown Mode
    //  true  = display off, data preserved
    //  false = display on
    // ─────────────────────────────────────

    void setShutdown(bool off) {
        _shutdown = off;
        for (uint8_t c = 0; c < _chains; c++) {
            _sendReg(c, MAX7219_REG_SHUTDOWN, off ? 0x00 : 0x01);
        }
    }


    // ─────────────────────────────────────
    //  Test Mode
    //  lights all segments at full brightness
    // ─────────────────────────────────────

    void setTestMode(bool on) {
        for (uint8_t c = 0; c < _chains; c++) {
            _sendReg(c, MAX7219_REG_DISPLAYTEST, on ? 0x01 : 0x00);
        }
    }


    // ─────────────────────────────────────
    //  Scroll
    // ─────────────────────────────────────

    void scroll(const char* str,
                uint16_t speedMs       = 300,
                XSeg_ScrollDir dir     = XSEG_SCROLL_LEFT) {
        _scrollStr     = str;
        _scrollSpeed   = speedMs;
        _scrollDir     = dir;
        _scrollEnabled = true;
    }


private:

    // ─────────────────────────────────────
    //  Private State
    // ─────────────────────────────────────

    uint8_t      _cs;
    int8_t       _clk;
    int8_t       _din;
    uint8_t      _chains;
    bool         _softSPI;
    bool         _shutdown;
    XSeg_Buffer  _buffer;
    const char*  _scrollStr = nullptr;


    // ─────────────────────────────────────
    //  Flush Buffer to Hardware
    // ─────────────────────────────────────

    void _flush() {
        for (uint8_t i = 0; i < _numDigits; i++) {
            uint8_t reg  = MAX7219_REG_DIGIT0 + i;
            uint8_t mask = (uint8_t)_buffer.getDigit(i);
            for (uint8_t c = 0; c < _chains; c++) {
                _sendReg(c, reg, mask);
            }
        }
    }

    void _sendBlank() {
        for (uint8_t i = 0; i < _numDigits; i++) {
            uint8_t reg = MAX7219_REG_DIGIT0 + i;
            for (uint8_t c = 0; c < _chains; c++) {
                _sendReg(c, reg, 0x00);
            }
        }
    }


    // ─────────────────────────────────────
    //  SPI Send
    //  handles both hardware and soft SPI
    //  sends 16 bit packet — reg + data
    // ─────────────────────────────────────

    void _sendReg(uint8_t chain, uint8_t reg, uint8_t data) {
        XSEG_PIN_LOW(_cs);

        // for chained chips send NOOP to others
        for (uint8_t c = 0; c < _chains; c++) {
            if (c == chain) {
                _sendByte(reg);
                _sendByte(data);
            } else {
                _sendByte(MAX7219_REG_NOOP);
                _sendByte(0x00);
            }
        }

        XSEG_PIN_HIGH(_cs);
    }

    void _sendByte(uint8_t byte) {
        if (_softSPI) {
            _softSPISend(byte);
        } else {
            _hardSPISend(byte);
        }
    }


    // ─────────────────────────────────────
    //  Soft SPI — bit bang
    //  MSB first, mode 0
    // ─────────────────────────────────────

    void _softSPISend(uint8_t byte) {
        for (uint8_t i = 0; i < 8; i++) {
            if (byte & 0x80) {
                XSEG_PIN_HIGH(_din);
            } else {
                XSEG_PIN_LOW(_din);
            }
            byte <<= 1;
            XSEG_PIN_HIGH(_clk);
            XSEG_PIN_LOW(_clk);
        }
    }


    // ─────────────────────────────────────
    //  Hardware SPI
    //  platform aware
    // ─────────────────────────────────────

    void _initHardwareSPI() {
        #if defined(ARDUINO)
            SPI.begin();
            SPI.setDataMode(SPI_MODE0);
            SPI.setBitOrder(MSBFIRST);
            SPI.setClockDivider(SPI_CLOCK_DIV16);
        #endif
    }

    void _hardSPISend(uint8_t byte) {
        #if defined(ARDUINO)
            SPI.transfer(byte);
        #endif
    }
};
