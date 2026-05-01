#pragma once
#include <stdint.h>
#include <string.h>
#include "XSeg_Types.h"
#include "XSeg_Math.h"
#include "../fonts/XSeg_Font.h"

// ─────────────────────────────────────────
//  XSeg_Buffer.h
//  Display buffer management
//  Handles all formatting before
//  data reaches the driver
//
//  Responsibilities:
//  - digit buffer storage
//  - alignment (left, right, center)
//  - padding (space, leading zeros)
//  - negative number handling
//  - float formatting
//  - string formatting
//  - scrolling
//  - blinking
// ─────────────────────────────────────────

class XSeg_Buffer {
public:

    // ─────────────────────────────────────
    //  Constructor
    // ─────────────────────────────────────

    XSeg_Buffer(uint8_t numDigits, XSeg_Type type)
        : _numDigits(numDigits)
        , _type(type)
        , _dirty(false)
        , _blinkState(false)
        , _blinkCounter(0)
        , _scrollPos(0)
        , _scrollCounter(0)
    {
        clear();
    }


    // ─────────────────────────────────────
    //  Clear
    // ─────────────────────────────────────

    void clear() {
        for (uint8_t i = 0; i < XSEG_MAX_DIGITS; i++) {
            _buffer[i] = XSEG_SPACE;
        }
        _dirty = true;
    }


    // ─────────────────────────────────────
    //  Integer Formatting
    // ─────────────────────────────────────

    void writeInt(int32_t n, XSeg_Align align, XSeg_Padding padding) {
        clear();

        bool negative = (n < 0);
        uint32_t val  = (uint32_t)(negative ? -n : n);

        uint8_t digitCount = _countDigits(val);
        uint8_t startPos   = _calcStartPos(digitCount + (negative ? 1 : 0), align);

        // write digits right to left
        for (uint8_t i = 0; i < digitCount; i++) {
            uint8_t pos  = startPos + digitCount - 1 - i;
            uint8_t d    = XSeg_Math::getDigitAt(val, i);
            _buffer[pos] = _getDigitMask(d);
        }

        // leading zeros or spaces
        if (padding == XSEG_PAD_ZERO) {
            for (uint8_t i = 0; i < startPos; i++) {
                _buffer[i] = _getDigitMask(0);
            }
        }

        // minus sign
        if (negative && startPos > 0) {
            _buffer[startPos - 1] = XSEG_MINUS;
        }

        _dirty = true;
    }

    void writeInt(uint32_t n, XSeg_Align align, XSeg_Padding padding) {
        writeInt((int32_t)n, align, padding);
    }

    void writeInt(int64_t n, XSeg_Align align, XSeg_Padding padding) {
        clear();

        bool negative  = (n < 0);
        uint64_t val   = (uint64_t)(negative ? -n : n);

        uint8_t digitCount = _countDigits64(val);
        uint8_t startPos   = _calcStartPos(digitCount + (negative ? 1 : 0), align);

        for (uint8_t i = 0; i < digitCount; i++) {
            uint8_t pos  = startPos + digitCount - 1 - i;
            uint8_t d    = XSeg_Math::getDigitAt(val, i);
            _buffer[pos] = _getDigitMask(d);
        }

        if (padding == XSEG_PAD_ZERO) {
            for (uint8_t i = 0; i < startPos; i++) {
                _buffer[i] = _getDigitMask(0);
            }
        }

        if (negative && startPos > 0) {
            _buffer[startPos - 1] = XSEG_MINUS;
        }

        _dirty = true;
    }

    void writeInt(uint64_t n, XSeg_Align align, XSeg_Padding padding) {
        writeInt((int64_t)n, align, padding);
    }


    // ─────────────────────────────────────
    //  Float Formatting
    // ─────────────────────────────────────

    void writeFloat(float n, uint8_t decimals, XSeg_Align align) {
        clear();

        bool negative  = (n < 0);
        float val      = negative ? -n : n;

        // scale to integer
        uint32_t scaled = (uint32_t)(val * XSeg_Math::fast10Pow(decimals) + 0.5f);

        uint8_t digitCount = _countDigits(scaled);
        // ensure enough digits for decimals
        if (digitCount <= decimals) digitCount = decimals + 1;

        uint8_t totalNeeded = digitCount + (negative ? 1 : 0);
        uint8_t startPos    = _calcStartPos(totalNeeded, align);

        // write all digits
        for (uint8_t i = 0; i < digitCount; i++) {
            uint8_t pos  = startPos + digitCount - 1 - i;
            uint8_t d    = XSeg_Math::getDigitAt(scaled, i);
            _buffer[pos] = _getDigitMask(d);

            // place decimal point
            if (i == decimals && _type == XSEG_TYPE_8) {
                _buffer[pos] |= XSEG_SEG_DP;
            }
        }

        if (negative && startPos > 0) {
            _buffer[startPos - 1] = XSEG_MINUS;
        }

        _dirty = true;
    }

    void writeFloat(double n, uint8_t decimals, XSeg_Align align) {
        writeFloat((float)n, decimals, align);
    }


    // ─────────────────────────────────────
    //  String Formatting
    // ─────────────────────────────────────

    void writeString(const char* str, XSeg_Align align) {
        clear();

        uint8_t len      = (uint8_t)strlen(str);
        uint8_t startPos = _calcStartPos(len, align);

        for (uint8_t i = 0; i < len && (startPos + i) < _numDigits; i++) {
            _buffer[startPos + i] = _getCharMask(str[i]);
        }

        _dirty = true;
    }


    // ─────────────────────────────────────
    //  Raw Digit Write
    // ─────────────────────────────────────

    void writeRaw(uint8_t pos, uint16_t mask) {
        if (pos < _numDigits) {
            _buffer[pos] = mask;
            _dirty = true;
        }
    }

    void writeRawAll(uint16_t* masks) {
        for (uint8_t i = 0; i < _numDigits; i++) {
            _buffer[i] = masks[i];
        }
        _dirty = true;
    }


    // ─────────────────────────────────────
    //  Scroll Tick
    //  call from update() every loop
    // ─────────────────────────────────────

    bool scrollTick(const char* str, uint16_t speedMs, XSeg_ScrollDir dir) {
        _scrollCounter++;
        if (_scrollCounter < speedMs) return false;
        _scrollCounter = 0;

        uint8_t len = (uint8_t)strlen(str);

        clear();
        for (uint8_t i = 0; i < _numDigits; i++) {
            uint8_t idx;
            if (dir == XSEG_SCROLL_LEFT) {
                idx = (_scrollPos + i) % (len + _numDigits);
            } else {
                idx = (len + _numDigits - _scrollPos + i) % (len + _numDigits);
            }

            if (idx < len) {
                _buffer[i] = _getCharMask(str[idx]);
            } else {
                _buffer[i] = XSEG_SPACE;
            }
        }

        _scrollPos++;
        if (_scrollPos >= len + _numDigits) _scrollPos = 0;

        _dirty = true;
        return true;
    }


    // ─────────────────────────────────────
    //  Blink Tick
    //  call from update() every loop
    // ─────────────────────────────────────

    bool blinkTick(XSeg_Blink mode) {
        if (mode == XSEG_BLINK_OFF) {
            _blinkState = true;
            return _blinkState;
        }

        uint16_t rate;
        switch (mode) {
            case XSEG_BLINK_SLOW:   rate = 1000; break;
            case XSEG_BLINK_MEDIUM: rate = 500;  break;
            case XSEG_BLINK_FAST:   rate = 250;  break;
            default:                rate = 500;  break;
        }

        _blinkCounter++;
        if (_blinkCounter >= rate) {
            _blinkCounter = 0;
            _blinkState   = !_blinkState;
            _dirty        = true;
        }

        return _blinkState;
    }


    // ─────────────────────────────────────
    //  Buffer Access
    // ─────────────────────────────────────

    uint16_t getDigit(uint8_t pos) const {
        if (pos < _numDigits) return _buffer[pos];
        return XSEG_SPACE;
    }

    uint16_t* getBuffer() {
        return _buffer;
    }

    bool isDirty() const { return _dirty; }

    void clearDirty() { _dirty = false; }


private:

    // ─────────────────────────────────────
    //  Constants
    // ─────────────────────────────────────

    static const uint8_t XSEG_MAX_DIGITS = 16;


    // ─────────────────────────────────────
    //  State
    // ─────────────────────────────────────

    uint16_t      _buffer[XSEG_MAX_DIGITS];
    uint8_t       _numDigits;
    XSeg_Type     _type;
    bool          _dirty;
    bool          _blinkState;
    uint16_t      _blinkCounter;
    uint8_t       _scrollPos;
    uint16_t      _scrollCounter;


    // ─────────────────────────────────────
    //  Private Helpers
    // ─────────────────────────────────────

    uint8_t _countDigits(uint32_t n) const {
        if (n == 0) return 1;
        uint8_t count = 0;
        while (n > 0) { n /= 10; count++; }
        return count;
    }

    uint8_t _countDigits64(uint64_t n) const {
        if (n == 0) return 1;
        uint8_t count = 0;
        while (n > 0) { n /= 10; count++; }
        return count;
    }

    uint8_t _calcStartPos(uint8_t needed, XSeg_Align align) const {
        if (needed >= _numDigits) return 0;
        switch (align) {
            case XSEG_ALIGN_RIGHT:  return _numDigits - needed;
            case XSEG_ALIGN_LEFT:   return 0;
            case XSEG_ALIGN_CENTER: return (_numDigits - needed) / 2;
            default:                return 0;
        }
    }

    uint16_t _getDigitMask(uint8_t digit) const {
        switch (_type) {
            case XSEG_TYPE_7:  return XSeg_GetFont7 ('0' + digit);
            case XSEG_TYPE_8:  return XSeg_GetFont8 ('0' + digit);
            case XSEG_TYPE_14: return XSeg_GetFont14('0' + digit);
            case XSEG_TYPE_16: return XSeg_GetFont16('0' + digit);
            default:           return XSEG_SPACE;
        }
    }

    uint16_t _getCharMask(char c) const {
        switch (_type) {
            case XSEG_TYPE_7:  return XSeg_GetFont7 (c);
            case XSEG_TYPE_8:  return XSeg_GetFont8 (c);
            case XSEG_TYPE_14: return XSeg_GetFont14(c);
            case XSEG_TYPE_16: return XSeg_GetFont16(c);
            default:           return XSEG_SPACE;
        }
    }
};
