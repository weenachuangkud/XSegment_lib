#pragma once

// ─────────────────────────────────────────
//  XSegment_lib.h
//  Master include — add this to your sketch
//  #include <XSegment_lib.h>
//
//  XSegment_lib — Universal segment display
//  library for Arduino, ESP32, STM32, RP2040
//
//  Supports:
//  - 7, 8, 14, 16 segment displays
//  - TM1637, TM1638, MAX7219, HT16K33, GPIO
//  - Common anode and cathode
//  - AVR, ESP32, STM32, RP2040 platforms
// ─────────────────────────────────────────


// ─────────────────────────────────────────
//  Version
// ─────────────────────────────────────────

#define XSEGMENT_LIB_VERSION_MAJOR  1
#define XSEGMENT_LIB_VERSION_MINOR  0
#define XSEGMENT_LIB_VERSION_PATCH  0
#define XSEGMENT_LIB_VERSION        "1.0.0"


// ─────────────────────────────────────────
//  Core
// ─────────────────────────────────────────

#include "core/XSeg_Types.h"
#include "core/XSeg_Math.h"
#include "core/XSeg_Base.h"
#include "core/XSeg_Buffer.h"


// ─────────────────────────────────────────
//  Platform
// ─────────────────────────────────────────

#include "platform/XSeg_Platform.h"


// ─────────────────────────────────────────
//  Fonts
// ─────────────────────────────────────────

#include "fonts/XSeg_Font.h"


// ─────────────────────────────────────────
//  Drivers
// ─────────────────────────────────────────

#include "drivers/XSeg_TM1637.h"
#include "drivers/XSeg_TM1638.h"
#include "drivers/XSeg_MAX7219.h"
#include "drivers/XSeg_HT16K33.h"
#include "drivers/XSeg_GPIO.h"
