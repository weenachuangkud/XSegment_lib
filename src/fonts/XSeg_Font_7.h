#pragma once
#include <stdint.h>

#ifdef __AVR__
    #include <avr/pgmspace.h>
    #define XSEG_FONT_ATTR PROGMEM
#else
    #define XSEG_FONT_ATTR
#endif

// ─────────────────────────────────────────
//  7 Segment Layout
// ─────────────────────────────────────────
//
//      aaa
//     f   b
//     f   b
//      ggg
//     e   c
//     e   c
//      ddd  dp
//
//  bit:  7    6    5    4    3    2    1    0
//  seg:  dp   g    f    e    d    c    b    a
// ─────────────────────────────────────────

// digits 0-9
static const uint8_t XSEG_FONT_ATTR XSeg_Font7_Digits[10] = {
    0b00111111,  // 0 — a,b,c,d,e,f
    0b00000110,  // 1 — b,c
    0b01011011,  // 2 — a,b,d,e,g
    0b01001111,  // 3 — a,b,c,d,g
    0b01100110,  // 4 — b,c,f,g
    0b01101101,  // 5 — a,c,d,f,g
    0b01111101,  // 6 — a,c,d,e,f,g
    0b00000111,  // 7 — a,b,c
    0b01111111,  // 8 — all
    0b01101111,  // 9 — a,b,c,d,f,g
};

// letters A-Z (what 7seg can reasonably show)
static const uint8_t XSEG_FONT_ATTR XSeg_Font7_Alpha[26] = {
    0b01110111,  // A
    0b01111100,  // b
    0b00111001,  // C
    0b01011110,  // d
    0b01111001,  // E
    0b01110001,  // F
    0b00111101,  // G
    0b01110110,  // H
    0b00000110,  // I
    0b00011110,  // J
    0b00000000,  // K — not possible on 7seg
    0b00111000,  // L
    0b00000000,  // M — not possible on 7seg
    0b01010100,  // n
    0b00111111,  // O (same as 0)
    0b01110011,  // P
    0b01100111,  // q
    0b01010000,  // r
    0b01101101,  // S (same as 5)
    0b01111000,  // t
    0b00111110,  // U
    0b00111110,  // V (same as U on 7seg)
    0b00000000,  // W — not possible on 7seg
    0b00000000,  // X — not possible on 7seg
    0b01101110,  // y
    0b01011011,  // Z (same as 2)
};

// special characters
static const uint8_t XSEG_FONT_ATTR XSeg_Font7_Special[] = {
    0b00000000,  // ' ' space
    0b01000000,  // '-' minus
    0b10000000,  // '.' decimal point
    0b01100011,  // '°' degree
    0b00111001,  // '[' 
    0b00001111,  // ']'
    0b01110110,  // 'H' for hex
    0b00000000,  // unknown → blank
};
