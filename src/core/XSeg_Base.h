#pragma once
#include <stdint.h>
#include "XSeg_Types.h"
#include "XSeg_Math.h"

// ─────────────────────────────────────────
//  XSeg_Base.h
//  Abstract base class all drivers inherit
//  Defines the common API contract
//  No hardware code here — pure interface
// ─────────────────────────────────────────

class XSeg_Base {
public:

    // ─────────────────────────────────────
    //  Constructor
    // ─────────────────────────────────────

    XSeg_Base(XSeg_Config config) 
        : _config(config)
        , _numDigits(config.digits)
        , _type(config.type)
        , _polarity(config.polarity)
        , _align(config.align)
        , _padding(config.padding)
        , _brightness(config.brightness)
        , _blinkMode(XSEG_BLINK_OFF)
        , _scrollDir(XSEG_SCROLL_LEFT)
        , _scrollSpeed(100)
        , _scrollEnabled(false)
    {}


    // ─────────────────────────────────────
    //  Lifecycle — must implement
    // ─────────────────────────────────────

    virtual void begin()   = 0;   // init hardware
    virtual void clear()   = 0;   // blank all digits
    virtual void update()  = 0;   // push buffer to display
                                  // call in loop() for multiplex drivers


    // ─────────────────────────────────────
    //  Show — must implement
    // ─────────────────────────────────────

    virtual void show(int32_t n)                    = 0;
    virtual void show(uint32_t n)                   = 0;
    virtual void show(int64_t n)                    = 0;
    virtual void show(uint64_t n)                   = 0;
    virtual void show(float n,   uint8_t decimals)  = 0;
    virtual void show(double n,  uint8_t decimals)  = 0;
    virtual void show(const char* str)              = 0;


    // ─────────────────────────────────────
    //  Raw Control — must implement
    // ─────────────────────────────────────

    // set a single digit by position with raw bitmask
    virtual void setDigit(uint8_t pos, uint16_t mask) = 0;

    // set all digits at once with raw bitmask array
    virtual void setAll(uint16_t* masks)              = 0;


    // ─────────────────────────────────────
    //  Brightness — must implement
    // ─────────────────────────────────────

    virtual void setBrightness(uint8_t level)         = 0;


    // ─────────────────────────────────────
    //  Settings — base handles these
    //  drivers inherit for free
    // ─────────────────────────────────────

    void setAlignment(XSeg_Align align) {
        _align = align;
    }

    void setPadding(XSeg_Padding padding) {
        _padding = padding;
    }

    void setBlink(XSeg_Blink mode) {
        _blinkMode = mode;
    }

    void setScrollSpeed(uint16_t ms) {
        _scrollSpeed = ms;
    }

    void setScrollDir(XSeg_ScrollDir dir) {
        _scrollDir = dir;
    }

    void enableScroll(bool enable) {
        _scrollEnabled = enable;
    }


    // ─────────────────────────────────────
    //  Getters
    // ─────────────────────────────────────

    uint8_t       getNumDigits()   const { return _numDigits;   }
    uint8_t       getBrightness()  const { return _brightness;  }
    XSeg_Type     getType()        const { return _type;        }
    XSeg_Polarity getPolarity()    const { return _polarity;    }
    XSeg_Align    getAlignment()   const { return _align;       }
    XSeg_Padding  getPadding()     const { return _padding;     }
    XSeg_Blink    getBlinkMode()   const { return _blinkMode;   }
    bool          isScrolling()    const { return _scrollEnabled;}


    // ─────────────────────────────────────
    //  Polarity Helper
    //  drivers call this to invert bitmask
    //  if display is common anode
    // ─────────────────────────────────────

    inline uint16_t applyPolarity(uint16_t mask) const {
        if (_polarity == XSEG_COMMON_ANODE)
            return ~mask;
        return mask;
    }

    inline uint8_t applyPolarity8(uint8_t mask) const {
        if (_polarity == XSEG_COMMON_ANODE)
            return ~mask;
        return mask;
    }


    // ─────────────────────────────────────
    //  Destructor
    // ─────────────────────────────────────

    virtual ~XSeg_Base() {}


protected:

    // ─────────────────────────────────────
    //  Protected State
    //  accessible by all drivers
    // ─────────────────────────────────────

    XSeg_Config   _config;
    uint8_t       _numDigits;
    uint8_t       _brightness;
    XSeg_Type     _type;
    XSeg_Polarity _polarity;
    XSeg_Align    _align;
    XSeg_Padding  _padding;
    XSeg_Blink    _blinkMode;
    XSeg_ScrollDir _scrollDir;
    uint16_t      _scrollSpeed;
    bool          _scrollEnabled;
};
