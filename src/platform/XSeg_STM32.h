#pragma once
#include "stm32_def.h"

// STM32 uses BSRR register
// Upper 16 bits = reset, lower 16 bits = set
// Atomic single cycle operation

#define XSEG_PIN_HIGH(pin)        \
    (digitalPinToPort(pin)->BSRR = digitalPinToBitMask(pin))

#define XSEG_PIN_LOW(pin)         \
    (digitalPinToPort(pin)->BSRR = (digitalPinToBitMask(pin) << 16))

#define XSEG_PIN_OUTPUT(pin)      \
    pinMode(pin, OUTPUT)

#define XSEG_PIN_READ(pin)        \
    (digitalPinToPort(pin)->IDR & digitalPinToBitMask(pin))

#define XSEG_PIN_TOGGLE(pin)      \
    (digitalPinToPort(pin)->ODR ^= digitalPinToBitMask(pin))

#define XSEG_PORT_WRITE(port, mask) \
    (port->ODR = mask)
