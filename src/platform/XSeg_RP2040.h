#pragma once
#include "hardware/gpio.h"

// RP2040 uses SIO hardware registers
// gpio_set and gpio_clr for atomic ops

#define XSEG_PIN_HIGH(pin)        \
    (sio_hw->gpio_set = (1ul << (pin)))

#define XSEG_PIN_LOW(pin)         \
    (sio_hw->gpio_clr = (1ul << (pin)))

#define XSEG_PIN_OUTPUT(pin)      \
    (sio_hw->gpio_oe_set = (1ul << (pin)))

#define XSEG_PIN_READ(pin)        \
    ((sio_hw->gpio_in >> (pin)) & 1u)

#define XSEG_PIN_TOGGLE(pin)      \
    (sio_hw->gpio_togl = (1ul << (pin)))

#define XSEG_PORT_WRITE(port, mask) \
    (sio_hw->gpio_set = (mask))
