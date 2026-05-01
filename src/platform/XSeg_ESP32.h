#pragma once

#define XSEG_PIN_HIGH(pin)        \
    digitalWrite(pin, HIGH)

#define XSEG_PIN_LOW(pin)         \
    digitalWrite(pin, LOW)

#define XSEG_PIN_OUTPUT(pin)      \
    pinMode(pin, OUTPUT)

#define XSEG_PIN_READ(pin)        \
    digitalRead(pin)

#define XSEG_PIN_TOGGLE(pin)      \
    digitalToggle(pin)

#define XSEG_PORT_WRITE(port, mask) \
    digitalWrite(port, mask)
