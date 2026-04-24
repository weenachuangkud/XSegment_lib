#pragma once
#include <stdint.h>

#ifdef __AVR__
    #include <avr/pgmspace.h>
    #define XSEG_FONT_ATTR PROGMEM
#else
    #define XSEG_FONT_ATTR
#endif

// ─────────────────────────────────────────
//  8 Segment Layout
//  Same as 7 segment + decimal point bit
// ─────────────────────────────────────────
//
//      aaa
//     f   b
//     f   b
//      ggg
//     e   c
//     e   c
//      ddd  .dp
//
//  bit:  7    6    5    4    3    2    1    0
//  seg:  dp   g    f    e    d    c    b    a
// ─────────────────────────────────────────

// digits 0-9 without decimal point
static const uint8_t XSEG_FONT_ATTR XSeg_Font8_Digits[10] = {
    0b00111111,  // 0
    0b00000110,  // 1
    0b01011011,  // 2
    0b01001111,  // 3
    0b01100110,  // 4
    0b01101101,  // 5
    0b01111101,  // 6
    0b00000111,  // 7
    0b01111111,  // 8
    0b01101111,  // 9
};

// digits 0-9 with decimal point lit
static const uint8_t XSEG_FONT_ATTR XSeg_Font8_Digits_DP[10] = {
    0b10111111,  // 0.
    0b10000110,  // 1.
    0b11011011,  // 2.
    0b11001111,  // 3.
    0b11100110,  // 4.
    0b11101101,  // 5.
    0b11111101,  // 6.
    0b10000111,  // 7.
    0b11111111,  // 8.
    0b11101111,  // 9.
};

// letters A-Z
static const uint8_t XSEG_FONT_ATTR XSeg_Font8_Alpha[26] = {
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
    0b00000000,  // K — not possible
    0b00111000,  // L
    0b00000000,  // M — not possible
    0b01010100,  // n
    0b00111111,  // O
    0b01110011,  // P
    0b01100111,  // q
    0b01010000,  // r
    0b01101101,  // S
    0b01111000,  // t
    0b00111110,  // U
    0b00111110,  // V
    0b00000000,  // W — not possible
    0b00000000,  // X — not possible
    0b01101110,  // y
    0b01011011,  // Z
};

// special characters
static const uint8_t XSEG_FONT_ATTR XSeg_Font8_Special[] = {
    0b00000000,  // ' ' space
    0b01000000,  // '-' minus
    0b10000000,  // '.' decimal point only
    0b01100011,  // '°' degree
    0b00111001,  // '['
    0b00001111,  // ']'
    0b00000000,  // unknown → blank
};
