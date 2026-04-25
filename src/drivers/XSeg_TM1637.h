#pragma once
#include <stdint.h>
#include "../core/XSeg_Types.h"
#include "../core/XSeg_Math.h"
#include "../core/XSeg_Base.h"
#include "../core/XSeg_Buffer.h"
#include "../fonts/XSeg_Font.h"
#include "../platform/XSeg_Platform.h"

// ─────────────────────────────────────────
//  XSeg_TM1637.h
//  Driver for TM1637 pseudo I2C displays
//  Commonly found as 4 digit clock modules
//
//  Features:
//  - 4 or 6 digit support
//  - brightness control (0-7)
//  - colon control (clock displays)
//  - non blocking
//  - common cathode only (TM1637 is fixed)
// ─────────────────────────────────────────

class XSeg_TM1637 : public XSeg_Base {
public:

    // ─────────────────────────────────────
    //  Constructor
    // ─────────────────────────────────────

    XSeg_TM1637(uint8_t clk, uint8_t dio, uint8_t digits = 4)
        : XSeg_Base({
            XSEG_TYPE_8,            // TM1637 is 8 seg (7 + dp)
            XSEG_COMMON_CATHODE,    // TM1637 fixed cathode
            XSEG_ALIGN_RIGHT,
            XSEG_PAD_SPACE,
            digits,
            7                       // max brightness default
          })
        , _clk(clk)
        , _dio(dio)
        , _colonOn(false)
        , _buffer(digits, XSEG_TYPE_8)
    {}


    // ─────────────────────────────────────
    //  Lifecycle
    // ─────────────────────────────────────

    void begin() override {
        XSEG_PIN_OUTPUT(_clk);
        XSEG_PIN_OUTPUT(_dio);
        XSEG_PIN_HIGH(_clk);
        XSEG_PIN_HIGH(_dio);
        setBrightness(_brightness);
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
        _buffer.writeFloat(n, decimals, _align, _padding);
    }

    void show(double n, uint8_t decimals) override {
        _scrollEnabled = false;
        _buffer.writeFloat((float)n, decimals, _align, _padding);
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
    //  0 = dimmest, 7 = brightest
    // ─────────────────────────────────────

    void setBrightness(uint8_t level) override {
        _brightness = XSeg_Math::clamp(level, 0, 7);
        // TM1637 brightness command
        // 0x88 = display on + brightness bits
        _sendCmd(0x88 | _brightness);
    }


    // ─────────────────────────────────────
    //  Colon
    //  TM1637 clock modules have a colon
    //  between digit 1 and 2
    // ─────────────────────────────────────

    void setColon(bool on) {
        _colonOn = on;
        _buffer.writeRaw(1, _buffer.getDigit(1) | (on ? 0x80 : 0x00));
    }

    bool getColon() const {
        return _colonOn;
    }


    // ─────────────────────────────────────
    //  Scroll String
    // ─────────────────────────────────────

    void scroll(const char* str, uint16_t speedMs = 300,
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

    uint8_t       _clk;
    uint8_t       _dio;
    bool          _colonOn;
    XSeg_Buffer   _buffer;
    const char*   _scrollStr   = nullptr;


    // ─────────────────────────────────────
    //  TM1637 Protocol
    //  pseudo I2C — bit banged manually
    //  timing is critical here
    // ─────────────────────────────────────

    void _start() {
        XSEG_PIN_HIGH(_clk);
        XSEG_PIN_HIGH(_dio);
        _delayUs(2);
        XSEG_PIN_LOW(_dio);
        _delayUs(2);
        XSEG_PIN_LOW(_clk);
    }

    void _stop() {
        XSEG_PIN_LOW(_clk);
        XSEG_PIN_LOW(_dio);
        _delayUs(2);
        XSEG_PIN_HIGH(_clk);
        _delayUs(2);
        XSEG_PIN_HIGH(_dio);
    }

    bool _writeByte(uint8_t byte) {
        for (uint8_t i = 0; i < 8; i++) {
            XSEG_PIN_LOW(_clk);
            _delayUs(2);

            if (byte & 0x01) {
                XSEG_PIN_HIGH(_dio);
            } else {
                XSEG_PIN_LOW(_dio);
            }

            byte >>= 1;
            _delayUs(2);
            XSEG_PIN_HIGH(_clk);
            _delayUs(2);
        }

        // wait for ACK
        XSEG_PIN_LOW(_clk);
        XSEG_PIN_HIGH(_dio);
        _delayUs(5);
        XSEG_PIN_HIGH(_clk);
        _delayUs(2);

        // read ACK bit
        bool ack = !XSEG_PIN_READ(_dio);

        XSEG_PIN_LOW(_clk);
        return ack;
    }

    void _sendCmd(uint8_t cmd) {
        _start();
        _writeByte(cmd);
        _stop();
    }

    void _flush() {
        // TM1637 write data command — auto increment address
        _sendCmd(0x40);

        // set starting address
        _start();
        _writeByte(0xC0);

        // write all digit data
        for (uint8_t i = 0; i < _numDigits; i++) {
            uint8_t mask = (uint8_t)_buffer.getDigit(i);
            _writeByte(mask);
        }

        _stop();

        // set brightness
        _sendCmd(0x88 | _brightness);
    }

    void _sendBlank() {
        _sendCmd(0x40);
        _start();
        _writeByte(0xC0);
        for (uint8_t i = 0; i < _numDigits; i++) {
            _writeByte(0x00);
        }
        _stop();
        _sendCmd(0x88 | _brightness);
    }


    // ─────────────────────────────────────
    //  Delay
    //  platform aware microsecond delay
    // ─────────────────────────────────────

    inline void _delayUs(uint8_t us) {
        #if defined(ESP32) || defined(ESP8266)
            ets_delay_us(us);
        #elif defined(__AVR__)
            _delay_us(us);
        #else
            delayMicroseconds(us);
        #endif
    }
};
