#pragma once
#include <stdint.h>
#include "../core/XSeg_Types.h"
#include "../core/XSeg_Math.h"
#include "../core/XSeg_Base.h"
#include "../core/XSeg_Buffer.h"
#include "../fonts/XSeg_Font.h"
#include "../platform/XSeg_Platform.h"

// ─────────────────────────────────────────
//  XSeg_GPIO.h
//  Raw register-based GPIO driver
//  Supports any segment type 7/8/14/16
//  Supports multiplexed multi digit displays
//  Supports common anode and cathode
//  No driver chip needed — direct wiring
//
//  Features:
//  - register based — fastest possible
//  - any segment type 7/8/14/16
//  - common anode and cathode
//  - single or multi digit
//  - multiplexed refresh
//  - configurable refresh rate
//  - non blocking
// ─────────────────────────────────────────

// max segments supported
#define XSEG_GPIO_MAX_SEGMENTS  16
#define XSEG_GPIO_MAX_DIGITS    8


class XSeg_GPIO : public XSeg_Base {
public:

    // ─────────────────────────────────────
    //  Constructor
    //
    //  segPins   — array of segment pins
    //              7seg:  {a,b,c,d,e,f,g}
    //              8seg:  {a,b,c,d,e,f,g,dp}
    //              14seg: {a,b,c,d,e,f,g,h,i,j,k,l,m,n}
    //              16seg: {a1,a2,b,c,d1,d2,e,f,g1,g2,h,i,j,k,l,m}
    //
    //  digitPins — array of digit select pins
    //              one per digit
    //              nullptr if single digit display
    //
    //  numDigits — number of digits
    //  type      — XSEG_TYPE_7/8/14/16
    //  polarity  — XSEG_COMMON_CATHODE or XSEG_COMMON_ANODE
    // ─────────────────────────────────────

    XSeg_GPIO(uint8_t*      segPins,
              uint8_t*      digitPins,
              uint8_t       numDigits,
              XSeg_Type     type     = XSEG_TYPE_7,
              XSeg_Polarity polarity = XSEG_COMMON_CATHODE)
        : XSeg_Base({
            type,
            polarity,
            XSEG_ALIGN_RIGHT,
            XSEG_PAD_SPACE,
            numDigits,
            7
          })
        , _digitPins(digitPins)
        , _currentDigit(0)
        , _refreshRate(2000)        // 2000us = 500Hz default
        , _lastRefresh(0)
        , _buffer(numDigits, type)
    {
        // copy segment pins
        uint8_t segCount = _segCount();
        for (uint8_t i = 0; i < segCount; i++) {
            _segPins[i] = segPins[i];
        }
    }


    // ─────────────────────────────────────
    //  Lifecycle
    // ─────────────────────────────────────

    void begin() override {
        uint8_t segCount = _segCount();

        // set all segment pins as output
        for (uint8_t i = 0; i < segCount; i++) {
            XSEG_PIN_OUTPUT(_segPins[i]);
            XSEG_PIN_LOW(_segPins[i]);
        }

        // set all digit pins as output
        if (_digitPins != nullptr) {
            for (uint8_t i = 0; i < _numDigits; i++) {
                XSEG_PIN_OUTPUT(_digitPins[i]);
                // deselect all digits
                if (_polarity == XSEG_COMMON_CATHODE) {
                    XSEG_PIN_HIGH(_digitPins[i]);
                } else {
                    XSEG_PIN_LOW(_digitPins[i]);
                }
            }
        }

        clear();
    }

    void clear() override {
        _buffer.clear();
        _clearSegments();
    }

    // ─────────────────────────────────────
    //  Update
    //  MUST be called in loop()
    //  handles multiplexing
    // ─────────────────────────────────────

    void update() override {
        // blink handling
        if (!_buffer.blinkTick(_blinkMode)) {
            _clearSegments();
            return;
        }

        // scroll handling
        if (_scrollEnabled) {
            _buffer.scrollTick(_scrollStr, _scrollSpeed, _scrollDir);
        }

        // multiplex refresh
        _multiplexTick();
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
    //  GPIO has no hardware brightness
    //  simulated via PWM duty cycle on digit pins
    //  0 = off, 7 = full on
    // ─────────────────────────────────────

    void setBrightness(uint8_t level) override {
        _brightness = XSeg_Math::clamp(level, 0, 7);
        // brightness affects how long each digit
        // stays on during multiplex cycle
        // higher = longer on time = brighter
        _refreshRate = XSeg_Math::map(_brightness, 0, 7, 500, 2000);
    }


    // ─────────────────────────────────────
    //  Refresh Rate
    //  how often multiplex cycles in microseconds
    //  lower = faster refresh = less flicker
    //  but more CPU time
    //  default 2000us = 500Hz
    // ─────────────────────────────────────

    void setRefreshRate(uint16_t us) {
        _refreshRate = us;
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

    uint8_t      _segPins[XSEG_GPIO_MAX_SEGMENTS];
    uint8_t*     _digitPins;
    uint8_t      _currentDigit;
    uint16_t     _refreshRate;
    uint32_t     _lastRefresh;
    XSeg_Buffer  _buffer;
    const char*  _scrollStr = nullptr;


    // ─────────────────────────────────────
    //  Segment Count
    //  how many segment pins based on type
    // ─────────────────────────────────────

    uint8_t _segCount() const {
        switch (_type) {
            case XSEG_TYPE_7:  return 7;
            case XSEG_TYPE_8:  return 8;
            case XSEG_TYPE_14: return 14;
            case XSEG_TYPE_16: return 16;
            default:           return 7;
        }
    }


    // ─────────────────────────────────────
    //  Multiplex Tick
    //  called every loop iteration
    //  cycles through digits one at a time
    //  fast enough to appear simultaneous
    // ─────────────────────────────────────

    void _multiplexTick() {
        uint32_t now = _micros();
        if (now - _lastRefresh < _refreshRate) return;
        _lastRefresh = now;

        // turn off current digit
        _deactivateDigit(_currentDigit);

        // move to next digit
        _currentDigit++;
        if (_currentDigit >= _numDigits) _currentDigit = 0;

        // write segment data for new digit
        uint16_t mask = applyPolarity(_buffer.getDigit(_currentDigit));
        _writeSegments(mask);

        // turn on new digit
        _activateDigit(_currentDigit);
    }


    // ─────────────────────────────────────
    //  Segment Write
    //  writes bitmask to segment pins
    //  each bit = one segment pin
    // ─────────────────────────────────────

    void _writeSegments(uint16_t mask) {
        uint8_t segCount = _segCount();
        for (uint8_t i = 0; i < segCount; i++) {
            if (mask & (1 << i)) {
                XSEG_PIN_HIGH(_segPins[i]);
            } else {
                XSEG_PIN_LOW(_segPins[i]);
            }
        }
    }

    void _clearSegments() {
        uint8_t segCount = _segCount();
        for (uint8_t i = 0; i < segCount; i++) {
            if (_polarity == XSEG_COMMON_CATHODE) {
                XSEG_PIN_LOW(_segPins[i]);
            } else {
                XSEG_PIN_HIGH(_segPins[i]);
            }
        }
    }


    // ─────────────────────────────────────
    //  Digit Select
    //  activates or deactivates digit pin
    //  polarity aware
    // ─────────────────────────────────────

    void _activateDigit(uint8_t pos) {
        if (_digitPins == nullptr) return;
        if (_polarity == XSEG_COMMON_CATHODE) {
            XSEG_PIN_LOW(_digitPins[pos]);   // cathode — pull low to select
        } else {
            XSEG_PIN_HIGH(_digitPins[pos]);  // anode — pull high to select
        }
    }

    void _deactivateDigit(uint8_t pos) {
        if (_digitPins == nullptr) return;
        if (_polarity == XSEG_COMMON_CATHODE) {
            XSEG_PIN_HIGH(_digitPins[pos]);  // cathode — pull high to deselect
        } else {
            XSEG_PIN_LOW(_digitPins[pos]);   // anode — pull low to deselect
        }
    }


    // ─────────────────────────────────────
    //  Micros
    //  platform aware microsecond timer
    // ─────────────────────────────────────

    uint32_t _micros() {
        #if defined(ARDUINO)
            return micros();
        #elif defined(STM32)
            return HAL_GetTick() * 1000;
        #else
            return 0;
        #endif
    }
};
