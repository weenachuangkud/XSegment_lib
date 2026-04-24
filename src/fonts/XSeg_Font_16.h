#pragma once
#include <stdint.h>

#ifdef __AVR__
    #include <avr/pgmspace.h>
    #define XSEG_FONT_ATTR PROGMEM
#else
    #define XSEG_FONT_ATTR
#endif

// ─────────────────────────────────────────
//  16 Segment Layout
// ─────────────────────────────────────────
//
//    a1  a2
//   f|h  i|b
//   f|h  i|b
//    g1  g2
//   e|j  k|c
//   e|j  k|c
//    d1  d2  dp
//
//  bit:  15   14   13   12   11   10   9    8    7    6    5    4    3    2    1    0
//  seg:  dp   n    m    l    k    j    i    h    g2   g1   f    e    d2   d1   b    a2  a1
//
//  vs 14seg — splits a→a1,a2 and d→d1,d2
//  gives cleaner top/bottom on letters like A M T Y Z
// ─────────────────────────────────────────

// digits 0-9
static const uint16_t XSEG_FONT_ATTR XSeg_Font16_Digits[10] = {
    0b0000000000111111,  // 0
    0b0000000000000110,  // 1
    0b0000000001011011,  // 2
    0b0000000001001111,  // 3
    0b0000000001100110,  // 4
    0b0000000001101101,  // 5
    0b0000000001111101,  // 6
    0b0000000000000111,  // 7
    0b0000000001111111,  // 8
    0b0000000001101111,  // 9
};

// full A-Z with 16seg precision
static const uint16_t XSEG_FONT_ATTR XSeg_Font16_Alpha[26] = {
    0b0000000011110111,  // A — split top bar cleaner
    0b0000010010001111,  // B
    0b0000000000111001,  // C
    0b0000010000001111,  // D
    0b0000000011111001,  // E
    0b0000000011110001,  // F
    0b0000000010111101,  // G
    0b0000000011110110,  // H
    0b0000010100001001,  // I — uses verticals
    0b0000000000011110,  // J
    0b0000001011110000,  // K
    0b0000000000111000,  // L
    0b0000000110110110,  // M — split diagonals cleaner
    0b0000001000110110,  // N
    0b0000000000111111,  // O
    0b0000000011110011,  // P
    0b0000001000111111,  // Q
    0b0000001011110011,  // R
    0b0000000011101101,  // S
    0b0000010100000001,  // T — split top bar
    0b0000000000111110,  // U
    0b0000000100110000,  // V
    0b0000001100110110,  // W — split diagonals
    0b0000001110000000,  // X
    0b0000010100000110,  // Y
    0b0000000101001001,  // Z — split bars cleaner
};

// special characters
static const uint16_t XSEG_FONT_ATTR XSeg_Font16_Special[] = {
    0b0000000000000000,  // ' ' space
    0b0000000001000000,  // '-' minus
    0b1000000000000000,  // '.' decimal point
    0b0000000001100011,  // '°' degree
    0b0000000000111001,  // '['
    0b0000000000001111,  // ']'
    0b0000000000000000,  // unknown → blank
};
