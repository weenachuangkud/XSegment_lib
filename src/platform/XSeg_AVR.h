#pragma once
#include <avr/io.h>

// Direct PORT register manipulation
// Works on Uno, Nano, Mega, ATtiny...

#define XSEG_PIN_HIGH(pin)        \
    (*portOutputRegister(digitalPinToPort(pin)) |= digitalPinToBitMask(pin))

#define XSEG_PIN_LOW(pin)         \
    (*portOutputRegister(digitalPinToPort(pin)) &= ~digitalPinToBitMask(pin))

#define XSEG_PIN_OUTPUT(pin)      \
    (*portModeRegister(digitalPinToPort(pin)) |= digitalPinToBitMask(pin))

#define XSEG_PIN_READ(pin)        \
    (*portInputRegister(digitalPinToPort(pin)) & digitalPinToBitMask(pin))

#define XSEG_PIN_TOGGLE(pin)      \
    (*portInputRegister(digitalPinToPort(pin)) |= digitalPinToBitMask(pin))

#define XSEG_PORT_WRITE(port, mask) \
    (port = mask)
