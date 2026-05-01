#pragma once
// ─────────────────────────────────────────
//  Arduino.h stub for desktop unit tests
//  Replaces Arduino.h so tests compile
//  without Arduino SDK installed
// ─────────────────────────────────────────

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Arduino type aliases
typedef bool     boolean;
typedef uint8_t  byte;

// Arduino pin modes
#define INPUT    0
#define OUTPUT   1
#define INPUT_PULLUP 2

// Arduino pin states
#define LOW      0
#define HIGH     1

// Arduino math
#define PI       3.14159265358979323846

// Arduino delay stubs — do nothing on desktop
inline void delay(unsigned long ms)        { (void)ms; }
inline void delayMicroseconds(unsigned int us) { (void)us; }

// Arduino pin stubs — do nothing on desktop
inline void pinMode(uint8_t pin, uint8_t mode)       { (void)pin; (void)mode; }
inline void digitalWrite(uint8_t pin, uint8_t val)   { (void)pin; (void)val; }
inline int  digitalRead(uint8_t pin)                 { (void)pin; return 0; }

// Arduino Serial stub
struct SerialStub {
    void begin(long baud)          { (void)baud; }
    void print(const char* s)      { (void)s; }
    void println(const char* s)    { (void)s; }
    void print(int n)              { (void)n; }
    void println(int n)            { (void)n; }
};
extern SerialStub Serial;

// Wire stub
struct WireStub {
    void begin()                   {}
    void begin(int sda, int scl)   { (void)sda; (void)scl; }
    void setClock(long freq)       { (void)freq; }
    void beginTransmission(int addr) { (void)addr; }
    void write(uint8_t data)       { (void)data; }
    void endTransmission()         {}
};
extern WireStub Wire;

// SPI stub
struct SPIStub {
    void begin()                   {}
    void setDataMode(uint8_t mode) { (void)mode; }
    void setBitOrder(uint8_t order){ (void)order; }
    void setClockDivider(uint8_t d){ (void)d; }
    uint8_t transfer(uint8_t data) { (void)data; return 0; }
};
extern SPIStub SPI;

// SPI constants
#define SPI_MODE0           0
#define SPI_CLOCK_DIV16     4
#define MSBFIRST            1
#define LSBFIRST            0
