#pragma once

//  Platform Detection

#if defined(__AVR__)
    #include "XSeg_AVR.h"
    #define XSEG_PLATFORM "AVR"

#elif defined(ESP32)
    #include "XSeg_ESP32.h"
    #define XSEG_PLATFORM "ESP32"

#elif defined(ESP8266)
    #include "XSeg_ESP8266.h"
    #define XSEG_PLATFORM "ESP8266"

#elif defined(STM32)
    #include "XSeg_STM32.h"
    #define XSEG_PLATFORM "STM32"

#elif defined(ARDUINO_ARCH_RP2040)
    #include "XSeg_RP2040.h"
    #define XSEG_PLATFORM "RP2040"

#elif defined(ARDUINO_ARCH_SAMD)
    #include "XSeg_SAMD.h"
    #define XSEG_PLATFORM "SAMD"

#else
    // fallback — use digitalWrite, no registers
    #include "XSeg_Generic.h"
    #define XSEG_PLATFORM "GENERIC"
    #warning "XSegment_lib: Unknown platform, falling back to generic digitalWrite driver. Performance may be reduced."

#endif


//  Shared Platform Macros
//  each platform file must define these

// XSEG_PIN_HIGH(pin)       set pin HIGH via register
// XSEG_PIN_LOW(pin)        set pin LOW via register
// XSEG_PIN_OUTPUT(pin)     set pin as OUTPUT via register
// XSEG_PIN_READ(pin)       read pin state via register
// XSEG_PIN_TOGGLE(pin)     toggle pin via register
// XSEG_PORT_WRITE(port, mask)   write full port at once

// Debugging
#ifdef XSEG_DEBUG
    #pragma message("XSegment_lib platform: " XSEG_PLATFORM)
#endif
