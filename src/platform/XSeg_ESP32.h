#pragma once
#include <driver/gpio.h>

#define XSEG_PIN_HIGH(pin)        \
    (GPIO.out_w1ts = (1UL << (pin)))

#define XSEG_PIN_LOW(pin)         \
    (GPIO.out_w1tc = (1UL << (pin)))

#define XSEG_PIN_OUTPUT(pin)      \
    do { gpio_reset_pin((gpio_num_t)pin); gpio_set_direction((gpio_num_t)pin, GPIO_MODE_OUTPUT); } while(0)

#define XSEG_PIN_READ(pin)        \
    gpio_get_level((gpio_num_t)pin)

#define XSEG_PIN_TOGGLE(pin)       \
    digitalToggle(pin)

#define XSEG_PORT_WRITE(port, mask) \
    (GPIO.out_w1ts = (mask))
