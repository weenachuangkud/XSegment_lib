#pragma once
#include <stdint.h>

// ─────────────────────────────────────────
//  XSeg_Types.h
//  All enums, structs, constants, bit defines
//  Include this everywhere you need type info
// ─────────────────────────────────────────


// ─────────────────────────────────────────
//  Display Type
// ─────────────────────────────────────────

enum XSeg_Type : uint8_t {
    XSEG_TYPE_7  = 7,
    XSEG_TYPE_8  = 8,
    XSEG_TYPE_14 = 14,
    XSEG_TYPE_16 = 16,
};


// ─────────────────────────────────────────
//  Polarity
// ─────────────────────────────────────────

enum XSeg_Polarity : uint8_t {
    XSEG_COMMON_CATHODE = 0,
    XSEG_COMMON_ANODE   = 1,
};


// ─────────────────────────────────────────
//  Alignment
// ─────────────────────────────────────────

enum XSeg_Align : uint8_t {
    XSEG_ALIGN_RIGHT  = 0,   // default for numbers
    XSEG_ALIGN_LEFT   = 1,   // default for strings
    XSEG_ALIGN_CENTER = 2,
};


// ─────────────────────────────────────────
//  Padding
// ─────────────────────────────────────────

enum XSeg_Padding : uint8_t {
    XSEG_PAD_SPACE = 0,   // blank unused digits
    XSEG_PAD_ZERO  = 1,   // leading zeros
};


// ─────────────────────────────────────────
//  Scroll Direction
// ─────────────────────────────────────────

enum XSeg_ScrollDir : uint8_t {
    XSEG_SCROLL_LEFT  = 0,
    XSEG_SCROLL_RIGHT = 1,
};


// ─────────────────────────────────────────
//  Blink Mode
// ─────────────────────────────────────────

enum XSeg_Blink : uint8_t {
    XSEG_BLINK_OFF    = 0,
    XSEG_BLINK_SLOW   = 1,   // ~0.5 Hz
    XSEG_BLINK_MEDIUM = 2,   // ~1 Hz
    XSEG_BLINK_FAST   = 3,   // ~2 Hz
};


// ─────────────────────────────────────────
//  Display Config Struct
//  Passed into driver constructor
// ─────────────────────────────────────────

struct XSeg_Config {
    XSeg_Config() {}
    XSeg_Config(XSeg_Type t, XSeg_Polarity p, XSeg_Align a, XSeg_Padding pad, uint8_t d, uint8_t b)
        : type(t), polarity(p), align(a), padding(pad), digits(d), brightness(b) {}

    XSeg_Type     type      = XSEG_TYPE_7;
    XSeg_Polarity polarity  = XSEG_COMMON_CATHODE;
    XSeg_Align    align     = XSEG_ALIGN_RIGHT;
    XSeg_Padding  padding   = XSEG_PAD_SPACE;
    uint8_t       digits    = 4;
    uint8_t       brightness = 7;
};


// ─────────────────────────────────────────
//  7 / 8 Segment Bit Defines
// ─────────────────────────────────────────

#define XSEG_SEG_A      (1 << 0)    // 0b00000001  top horizontal
#define XSEG_SEG_B      (1 << 1)    // 0b00000010  top right vertical
#define XSEG_SEG_C      (1 << 2)    // 0b00000100  bottom right vertical
#define XSEG_SEG_D      (1 << 3)    // 0b00001000  bottom horizontal
#define XSEG_SEG_E      (1 << 4)    // 0b00010000  bottom left vertical
#define XSEG_SEG_F      (1 << 5)    // 0b00100000  top left vertical
#define XSEG_SEG_G      (1 << 6)    // 0b01000000  middle horizontal
#define XSEG_SEG_DP     (1 << 7)    // 0b10000000  decimal point


// ─────────────────────────────────────────
//  14 / 16 Segment Extra Bit Defines
// ─────────────────────────────────────────

#define XSEG_SEG_H      (1 << 8)    // top left diagonal
#define XSEG_SEG_I      (1 << 9)    // top vertical
#define XSEG_SEG_J      (1 << 10)   // middle vertical
#define XSEG_SEG_K      (1 << 11)   // top right diagonal
#define XSEG_SEG_L      (1 << 12)   // bottom right diagonal
#define XSEG_SEG_M      (1 << 13)   // bottom vertical
#define XSEG_SEG_N      (1 << 14)   // bottom left diagonal

// 16 seg only — splits a and d into left/right halves
#define XSEG_SEG_A1     (1 << 0)    // top left horizontal
#define XSEG_SEG_A2     (1 << 1)    // top right horizontal
#define XSEG_SEG_D1     (1 << 2)    // bottom left horizontal
#define XSEG_SEG_D2     (1 << 3)    // bottom right horizontal


// ─────────────────────────────────────────
//  Digit Defines  0-9
// ─────────────────────────────────────────

#define XSEG_0    (XSEG_SEG_A | XSEG_SEG_B | XSEG_SEG_C | XSEG_SEG_D | XSEG_SEG_E | XSEG_SEG_F)
#define XSEG_1    (XSEG_SEG_B | XSEG_SEG_C)
#define XSEG_2    (XSEG_SEG_A | XSEG_SEG_B | XSEG_SEG_D | XSEG_SEG_E | XSEG_SEG_G)
#define XSEG_3    (XSEG_SEG_A | XSEG_SEG_B | XSEG_SEG_C | XSEG_SEG_D | XSEG_SEG_G)
#define XSEG_4    (XSEG_SEG_B | XSEG_SEG_C | XSEG_SEG_F | XSEG_SEG_G)
#define XSEG_5    (XSEG_SEG_A | XSEG_SEG_C | XSEG_SEG_D | XSEG_SEG_F | XSEG_SEG_G)
#define XSEG_6    (XSEG_SEG_A | XSEG_SEG_C | XSEG_SEG_D | XSEG_SEG_E | XSEG_SEG_F | XSEG_SEG_G)
#define XSEG_7    (XSEG_SEG_A | XSEG_SEG_B | XSEG_SEG_C)
#define XSEG_8    (XSEG_SEG_A | XSEG_SEG_B | XSEG_SEG_C | XSEG_SEG_D | XSEG_SEG_E | XSEG_SEG_F | XSEG_SEG_G)
#define XSEG_9    (XSEG_SEG_A | XSEG_SEG_B | XSEG_SEG_C | XSEG_SEG_D | XSEG_SEG_F | XSEG_SEG_G)


// ─────────────────────────────────────────
//  Letter Defines  A-Z  (7seg versions)
// ─────────────────────────────────────────

#define XSEG_A    (XSEG_SEG_A | XSEG_SEG_B | XSEG_SEG_C | XSEG_SEG_E | XSEG_SEG_F | XSEG_SEG_G)
#define XSEG_B    (XSEG_SEG_C | XSEG_SEG_D | XSEG_SEG_E | XSEG_SEG_F | XSEG_SEG_G)
#define XSEG_C    (XSEG_SEG_A | XSEG_SEG_D | XSEG_SEG_E | XSEG_SEG_F)
#define XSEG_D    (XSEG_SEG_B | XSEG_SEG_C | XSEG_SEG_D | XSEG_SEG_E | XSEG_SEG_G)
#define XSEG_E    (XSEG_SEG_A | XSEG_SEG_D | XSEG_SEG_E | XSEG_SEG_F | XSEG_SEG_G)
#define XSEG_F    (XSEG_SEG_A | XSEG_SEG_E | XSEG_SEG_F | XSEG_SEG_G)
#define XSEG_G    (XSEG_SEG_A | XSEG_SEG_C | XSEG_SEG_D | XSEG_SEG_E | XSEG_SEG_F)
#define XSEG_H    (XSEG_SEG_B | XSEG_SEG_C | XSEG_SEG_E | XSEG_SEG_F | XSEG_SEG_G)
#define XSEG_I    (XSEG_SEG_B | XSEG_SEG_C)
#define XSEG_J    (XSEG_SEG_B | XSEG_SEG_C | XSEG_SEG_D | XSEG_SEG_E)
#define XSEG_K    0x0000  // not possible on 7seg
#define XSEG_L    (XSEG_SEG_D | XSEG_SEG_E | XSEG_SEG_F)
#define XSEG_M    0x0000  // not possible on 7seg
#define XSEG_N    (XSEG_SEG_C | XSEG_SEG_E | XSEG_SEG_G)
#define XSEG_O    (XSEG_SEG_A | XSEG_SEG_B | XSEG_SEG_C | XSEG_SEG_D | XSEG_SEG_E | XSEG_SEG_F)
#define XSEG_P    (XSEG_SEG_A | XSEG_SEG_B | XSEG_SEG_E | XSEG_SEG_F | XSEG_SEG_G)
#define XSEG_Q    (XSEG_SEG_A | XSEG_SEG_B | XSEG_SEG_C | XSEG_SEG_F | XSEG_SEG_G)
#define XSEG_R    (XSEG_SEG_E | XSEG_SEG_G)
#define XSEG_S    (XSEG_SEG_A | XSEG_SEG_C | XSEG_SEG_D | XSEG_SEG_F | XSEG_SEG_G)
#define XSEG_T    (XSEG_SEG_D | XSEG_SEG_E | XSEG_SEG_F | XSEG_SEG_G)
#define XSEG_U    (XSEG_SEG_B | XSEG_SEG_C | XSEG_SEG_D | XSEG_SEG_E | XSEG_SEG_F)
#define XSEG_V    (XSEG_SEG_C | XSEG_SEG_D | XSEG_SEG_E)
#define XSEG_W    0x0000  // not possible on 7seg
#define XSEG_X    0x0000  // not possible on 7seg
#define XSEG_Y    (XSEG_SEG_B | XSEG_SEG_C | XSEG_SEG_D | XSEG_SEG_F | XSEG_SEG_G)
#define XSEG_Z    (XSEG_SEG_A | XSEG_SEG_B | XSEG_SEG_D | XSEG_SEG_E | XSEG_SEG_G)


// ─────────────────────────────────────────
//  Special Character Defines
// ─────────────────────────────────────────

#define XSEG_SPACE      0x0000
#define XSEG_MINUS      (XSEG_SEG_G)
#define XSEG_DP         (XSEG_SEG_DP)
#define XSEG_DEGREE     (XSEG_SEG_A | XSEG_SEG_B | XSEG_SEG_F | XSEG_SEG_G)
#define XSEG_UNDERSCORE (XSEG_SEG_D)
#define XSEG_EQUALS     (XSEG_SEG_D | XSEG_SEG_G)
#define XSEG_ALL_ON     0xFF
#define XSEG_ALL_OFF    0x00


// ─────────────────────────────────────────
//  Brightness Range
// ─────────────────────────────────────────

#define XSEG_BRIGHTNESS_MIN  0
#define XSEG_BRIGHTNESS_MAX  7


// ─────────────────────────────────────────
//  Version
// ─────────────────────────────────────────

#define XSEG_VERSION_MAJOR  1
#define XSEG_VERSION_MINOR  0
#define XSEG_VERSION_PATCH  0
