#pragma once
#include <stdint.h>

#ifdef __AVR__
    #include <avr/pgmspace.h>
    #define XSEG_FONT_ATTR PROGMEM
#else
    #define XSEG_FONT_ATTR
#endif

// ─────────────────────────────────────────
//  14 Segment Layout
// ─────────────────────────────────────────
//
//      aaa
//     f|h|b
//     f|h|b
//      ggg  (g1 left, g2 right)
//     e|j|c  (i = top diagonal, j = vertical, k = bottom diagonal)
//     e|j|c
//      ddd  dp
//
//  bit: 15-14  13   12   11   10   9    8    7    6    5    4    3    2    1    0
//  seg:  dp    n    m    l    k    j    h    g2   g1   f    e    d    c    b    a
// ─────────────────────────────────────────

// digits 0-9 (same visual as 7seg, just wider bitmask)
static const uint16_t XSEG_FONT_ATTR XSeg_Font14_Digits[10] = {
    0b00000000000111111,  // 0
    0b00000000000000110,  // 1
    0b00000000001011011,  // 2
    0b00000000001001111,  // 3
    0b00000000001100110,  // 4
    0b00000000001101101,  // 5
    0b00000000001111101,  // 6
    0b00000000000000111,  // 7
    0b00000000001111111,  // 8
    0b00000000001101111,  // 9
};

// full A-Z — 14seg can do them all properly
static const uint16_t XSEG_FONT_ATTR XSeg_Font14_Alpha[26] = {
    0b00000000001110111,  // A
    0b00000100101001111,  // B
    0b00000000000111001,  // C
    0b00000100000001111,  // D
    0b00000000001111001,  // E
    0b00000000001110001,  // F
    0b00000000001101111,  // G  (actually 0b00000000000111101)
    0b00000000001110110,  // H
    0b00000100100001001,  // I
    0b00000000000011110,  // J
    0b00001001001110000,  // K
    0b00000000000111000,  // L
    0b00000000100110110,  // M — uses diagonals!
    0b00001000100110110,  // N — uses diagonal
    0b00000000000111111,  // O
    0b00000000001110011,  // P
    0b00001000000111111,  // Q
    0b00001000001110011,  // R
    0b00000000001101101,  // S
    0b00000100100000001,  // T
    0b00000000000111110,  // U
    0b00000010000110000,  // V — diagonals
    0b00001010000110110,  // W — diagonals
    0b00001010100000000,  // X — diagonals
    0b00000100100000110,  // Y
    0b00000010000001001,  // Z — diagonals
};

// special characters
static const uint16_t XSEG_FONT_ATTR XSeg_Font14_Special[] = {
    0b00000000000000000,  // ' ' space
    0b00000000001000000,  // '-' minus
    0b00000000010000000,  // '.' decimal point
   // 0b00000000001100011,  // '°' degree
    0b00000000000000000,  // unknown → blank
};
