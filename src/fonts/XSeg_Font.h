#pragma once

//  XSeg_Font.h
//  Master font include + PROGMEM read macros
//  Include this file in drivers instead of
//  individual font files

//  PROGMEM Read Macros
//  Hides platform differences from drivers

#ifdef __AVR__
    #include <avr/pgmspace.h>

    // 8 bit font read (7seg, 8seg)
    #define XSEG_READ_FONT8(table, idx) \
        pgm_read_byte(&table[idx])

    // 16 bit font read (14seg, 16seg)
    #define XSEG_READ_FONT16(table, idx) \
        pgm_read_word(&table[idx])

#else
    // ESP32, STM32, RP2040
    // flash directly readable, no special access needed

    #define XSEG_READ_FONT8(table, idx)  (table[idx])
    #define XSEG_READ_FONT16(table, idx) (table[idx])

#endif

// ─────────────────────────────────────────
//  Font Table Includes
// ─────────────────────────────────────────

#include "XSeg_Font_7.h"
#include "XSeg_Font_8.h"
#include "XSeg_Font_14.h"
#include "XSeg_Font_16.h"

// ─────────────────────────────────────────
//  Universal Font Lookup
//  Works for any segment type
//  Handles digits, letters, special chars
// ─────────────────────────────────────────

inline uint8_t XSeg_GetFont7(char c) {
    if (c >= '0' && c <= '9')
        return XSEG_READ_FONT8(XSeg_Font7_Digits, c - '0');
    if (c >= 'A' && c <= 'Z')
        return XSEG_READ_FONT8(XSeg_Font7_Alpha, c - 'A');
    if (c >= 'a' && c <= 'z')
        return XSEG_READ_FONT8(XSeg_Font7_Alpha, c - 'a');
    // special characters
    switch (c) {
        case ' ': return XSEG_SPACE;
        case '-': return XSEG_MINUS;
        case '.': return XSEG_DP;
        case '_': return XSEG_UNDERSCORE;
        case '=': return XSEG_EQUALS;
        case '°': return XSEG_DEGREE;
        default:  return XSEG_SPACE; // unknown → blank
    }
}

inline uint8_t XSeg_GetFont8(char c, bool dp = false) {
    if (c >= '0' && c <= '9') {
        if (dp)
            return XSEG_READ_FONT8(XSeg_Font8_Digits_DP, c - '0');
        return XSEG_READ_FONT8(XSeg_Font8_Digits, c - '0');
    }
    if (c >= 'A' && c <= 'Z')
        return XSEG_READ_FONT8(XSeg_Font8_Alpha, c - 'A');
    if (c >= 'a' && c <= 'z')
        return XSEG_READ_FONT8(XSeg_Font8_Alpha, c - 'a');
    switch (c) {
        case ' ': return XSEG_SPACE;
        case '-': return XSEG_MINUS;
        case '.': return XSEG_DP;
        case '_': return XSEG_UNDERSCORE;
        case '°': return XSEG_DEGREE;
        default:  return XSEG_SPACE;
    }
}

inline uint16_t XSeg_GetFont14(char c) {
    if (c >= '0' && c <= '9')
        return XSEG_READ_FONT16(XSeg_Font14_Digits, c - '0');
    if (c >= 'A' && c <= 'Z')
        return XSEG_READ_FONT16(XSeg_Font14_Alpha, c - 'A');
    if (c >= 'a' && c <= 'z')
        return XSEG_READ_FONT16(XSeg_Font14_Alpha, c - 'a');
    switch (c) {
        case ' ': return XSEG_SPACE;
        case '-': return XSEG_MINUS;
        case '.': return XSEG_DP;
        case '_': return XSEG_UNDERSCORE;
        case '°': return XSEG_DEGREE;
        default:  return XSEG_SPACE;
    }
}

inline uint16_t XSeg_GetFont16(char c) {
    if (c >= '0' && c <= '9')
        return XSEG_READ_FONT16(XSeg_Font16_Digits, c - '0');
    if (c >= 'A' && c <= 'Z')
        return XSEG_READ_FONT16(XSeg_Font16_Alpha, c - 'A');
    if (c >= 'a' && c <= 'z')
        return XSEG_READ_FONT16(XSeg_Font16_Alpha, c - 'a');
    switch (c) {
        case ' ': return XSEG_SPACE;
        case '-': return XSEG_MINUS;
        case '.': return XSEG_DP;
        case '_': return XSEG_UNDERSCORE;
        case '°': return XSEG_DEGREE;
        default:  return XSEG_SPACE;
    }
}
