#pragma once
#include <stdint.h>
#include "../core/XSeg_Types.h"
#include "../core/XSeg_Math.h"
#include "../core/XSeg_Base.h"
#include "../core/XSeg_Buffer.h"
#include "../fonts/XSeg_Font.h"
#include "../platform/XSeg_Platform.h"

// ─────────────────────────────────────────
//  XSeg_TM1638.h
//  Driver for TM1638 LED driver
//  Popular as 8 digit + 8 LED + 8 button
//  all in one module
//
//  Features:
//  - 8 digit 7 segment display
//  - 8 individual LEDs
//  - 8 push buttons
//  - brightness control (0-7)
//  - non blocking
//  - button state change detection
// ─────────────────────────────────────────

// ─────────────────────────────────────────
//  TM1638 Commands
// ─────────────────────────────────────────

#define TM1638_CMD_DATA_WRITE       0x40    // write data to display
#define TM1638_CMD_DATA_READ        0x42    // read key scan data
#define TM1638_CMD_ADDR_AUTO        0x40    // auto increment address
#define TM1638_CMD_ADDR_FIXED       0x44    // fixed address mode
#define TM1638_CMD_DISPLAY_ON       0x88    // display on + brightness
#define TM1638_CMD_DISPLAY_OFF      0x80    // display off
#define TM1638_CMD_ADDR_BASE        0xC0    // starting register address

// LED registers
#define TM1638_LED_BASE             0xC1    // first LED register
                                            // each LED is 2 bytes apart

// button masks
#define TM1638_BTN_1                0x01
#define TM1638_BTN_2                0x02
#define TM1638_BTN_3                0x04
#define TM1638_BTN_4                0x08
#define TM1638_BTN_5                0x10
#define TM1638_BTN_6                0x20
#define TM1638_BTN_7                0x40
#define TM1638_BTN_8                0x80


class XSeg_TM1638 : public XSeg_Base {
public:

    // ─────────────────────────────────────
    //  Constructor
    //  clk  — clock pin
    //  dio  — data pin (bidirectional)
    //  stb  — strobe pin (chip select)
    // ─────────────────────────────────────

    XSeg_TM1638(uint8_t clk, uint8_t dio, uint8_t stb)
        : XSeg_Base({
            XSEG_TYPE_8,
            XSEG_COMMON_CATHODE,
            XSEG_ALIGN_RIGHT,
            XSEG_PAD_SPACE,
            8,                      // TM1638 always 8 digits
            7                       // max brightness default
          })
        , _clk(clk)
        , _dio(dio)
        , _stb(stb)
        , _ledState(0x00)
        , _lastButtons(0x00)
        , _buffer(8, XSEG_TYPE_8)
    {}


    // ─────────────────────────────────────
    //  Lifecycle
    // ─────────────────────────────────────

    void begin() override {
        XSEG_PIN_OUTPUT(_clk);
        XSEG_PIN_OUTPUT(_dio);
        XSEG_PIN_OUTPUT(_stb);

        XSEG_PIN_HIGH(_clk);
        XSEG_PIN_HIGH(_dio);
        XSEG_PIN_HIGH(_stb);

        // display on at default brightness
        _sendCmd(TM1638_CMD_DISPLAY_ON | _brightness);

        clear();
        clearLEDs();
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
        _sendCmd(TM1638_CMD_DISPLAY_ON | _brightness);
    }


    // ─────────────────────────────────────
    //  LEDs
    //  TM1638 module has 8 individual LEDs
    //  pos  — 0 to 7
    //  on   — true = on, false = off
    // ─────────────────────────────────────

    void setLED(uint8_t pos, bool on) {
        if (pos > 7) return;

        if (on) {
            _ledState |=  (1 << pos);
        } else {
            _ledState &= ~(1 << pos);
        }

        // write single LED register
        // LEDs are at odd addresses C1, C3, C5...
        _sendCmd(TM1638_CMD_ADDR_FIXED);
        _beginTransfer();
        _sendByte(TM1638_LED_BASE + (pos << 1));
        _sendByte(on ? 0x01 : 0x00);
        _endTransfer();
    }

    void setLEDs(uint8_t mask) {
        _ledState = mask;
        for (uint8_t i = 0; i < 8; i++) {
            setLED(i, (mask >> i) & 0x01);
        }
    }

    void clearLEDs() {
        setLEDs(0x00);
    }

    void toggleLED(uint8_t pos) {
        if (pos > 7) return;
        setLED(pos, !((_ledState >> pos) & 0x01));
    }

    uint8_t getLEDState() const {
        return _ledState;
    }


    // ─────────────────────────────────────
    //  Buttons
    //  TM1638 can read 8 buttons
    //  returns bitmask of pressed buttons
    //  bit 0 = button 1 ... bit 7 = button 8
    // ─────────────────────────────────────

    uint8_t readButtons() {
        uint8_t buttons = 0;

        _sendCmd(TM1638_CMD_DATA_READ);

        // set DIO as input
        XSEG_PIN_OUTPUT(_dio);  // will be set input in _readByte

        _beginTransfer();
        for (uint8_t i = 0; i < 4; i++) {
            uint8_t b = _readByte();
            buttons  |= (b << i);
        }
        _endTransfer();

        _lastButtons = buttons;
        return buttons;
    }

    // returns true if specific button is pressed
    bool isPressed(uint8_t btn) {
        return (readButtons() & btn) != 0;
    }

    // returns buttons that changed since last read
    uint8_t getButtonChange() {
        uint8_t prev    = _lastButtons;
        uint8_t current = readButtons();
        return prev ^ current;
    }

    // returns buttons that were just pressed
    uint8_t getButtonPressed() {
        uint8_t prev    = _lastButtons;
        uint8_t current = readButtons();
        return (~prev) & current;
    }

    // returns buttons that were just released
    uint8_t getButtonReleased() {
        uint8_t prev    = _lastButtons;
        uint8_t current = readButtons();
        return prev & (~current);
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

    uint8_t      _clk;
    uint8_t      _dio;
    uint8_t      _stb;
    uint8_t      _ledState;
    uint8_t      _lastButtons;
    XSeg_Buffer  _buffer;
    const char*  _scrollStr = nullptr;


    // ─────────────────────────────────────
    //  Flush Buffer to Hardware
    // ─────────────────────────────────────

    void _flush() {
        // auto increment mode
        _sendCmd(TM1638_CMD_DATA_WRITE);

        _beginTransfer();
        _sendByte(TM1638_CMD_ADDR_BASE);

        for (uint8_t i = 0; i < _numDigits; i++) {
            uint8_t mask = (uint8_t)_buffer.getDigit(i);
            _sendByte(mask);        // segment data
            _sendByte((_ledState >> i) & 0x01 ? 0x01 : 0x00); // LED data
        }

        _endTransfer();
        _sendCmd(TM1638_CMD_DISPLAY_ON | _brightness);
    }

    void _sendBlank() {
        _sendCmd(TM1638_CMD_DATA_WRITE);
        _beginTransfer();
        _sendByte(TM1638_CMD_ADDR_BASE);
        for (uint8_t i = 0; i < _numDigits; i++) {
            _sendByte(0x00);
            _sendByte((_ledState >> i) & 0x01 ? 0x01 : 0x00);
        }
        _endTransfer();
        _sendCmd(TM1638_CMD_DISPLAY_ON | _brightness);
    }


    // ─────────────────────────────────────
    //  TM1638 Protocol
    //  SPI-like, STB is chip select
    //  CLK idles HIGH
    //  data changes on falling CLK edge
    // ─────────────────────────────────────

    void _sendCmd(uint8_t cmd) {
        _beginTransfer();
        _sendByte(cmd);
        _endTransfer();
    }

    void _beginTransfer() {
        XSEG_PIN_LOW(_stb);
        _delayUs(1);
    }

    void _endTransfer() {
        XSEG_PIN_HIGH(_stb);
        _delayUs(1);
    }

    void _sendByte(uint8_t byte) {
        XSEG_PIN_OUTPUT(_dio);
        for (uint8_t i = 0; i < 8; i++) {
            XSEG_PIN_LOW(_clk);
            if (byte & 0x01) {
                XSEG_PIN_HIGH(_dio);
            } else {
                XSEG_PIN_LOW(_dio);
            }
            byte >>= 1;
            _delayUs(1);
            XSEG_PIN_HIGH(_clk);
            _delayUs(1);
        }
    }

    uint8_t _readByte() {
        uint8_t byte = 0;

        // set DIO as input for reading
        #if defined(__AVR__)
            *portModeRegister(digitalPinToPort(_dio))
                &= ~digitalPinToBitMask(_dio);
        #elif defined(ESP32)
            gpio_set_direction((gpio_num_t)_dio, GPIO_MODE_INPUT);
        #else
            pinMode(_dio, INPUT);
        #endif

        for (uint8_t i = 0; i < 8; i++) {
            byte >>= 1;
            XSEG_PIN_LOW(_clk);
            _delayUs(1);
            if (XSEG_PIN_READ(_dio)) {
                byte |= 0x80;
            }
            XSEG_PIN_HIGH(_clk);
            _delayUs(1);
        }

        // restore DIO as output
        XSEG_PIN_OUTPUT(_dio);

        return byte;
    }


    // ─────────────────────────────────────
    //  Delay
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
