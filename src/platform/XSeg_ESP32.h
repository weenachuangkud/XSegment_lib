#pragma once
#include "driver/gpio.h"

// ESP32 uses SET/CLEAR registers
// Writing to w1ts sets bits, w1tc clears bits
// Much faster than digitalWrite

#define XSEG_PIN_HIGH(pin)        \
    (GPIO.out_w1ts = (1UL << (pin)))

#define XSEG_PIN_LOW(pin)         \
    (GPIO.out_w1tc = (1UL << (pin)))

#define XSEG_PIN_OUTPUT(pin)      \
    (GPIO.enable_w1ts = (1UL << (pin)))

#define XSEG_PIN_READ(pin)        \
    ((GPIO.in >> (pin)) & 0x1)

#define XSEG_PIN_TOGGLE(pin)      \
    ((GPIO.out >> (pin)) & 0x1) ? XSEG_PIN_LOW(pin) : XSEG_PIN_HIGH(pin)

#define XSEG_PORT_WRITE(port, mask) \
    (GPIO.out_w1ts = (mask))
