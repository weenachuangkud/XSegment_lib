# XSegment_lib

Universal segment display library for Arduino and embedded platforms. Supports 7, 8, 14 and 16 segment displays with TM1637, TM1638, MAX7219, HT16K33 and raw GPIO drivers. Works on AVR, ESP32, STM32 and RP2040 with a single unified API.

---

## Features

- **Multiple Drivers**: TM1637, TM1638, MAX7219, HT16K33, raw GPIO
- **Multiple Segment Types**: 7-segment, 8-segment (7+dp), 14-segment, 16-segment
- **Multiple Platforms**: AVR, ESP32, STM32, RP2040
- **Common Anode & Cathode**: Automatic polarity handling
- **Advanced Display**: Alignment, padding, blinking, scrolling
- **No Dependencies**: Pure Arduino/C++, no external libs needed

---

## Supported Hardware

| Driver | Interface | Segments | Max Digits | Notes |
|--------|-----------|----------|------------|-------|
| TM1637 | Pseudo I2C | 8 | 4-6 | Clock modules |
| TM1638 | Strobe I/O | 8 | 8 | With buttons/LEDs |
| MAX7219 | SPI | 8 | 8 per chip | Chainable |
| HT16K33 | I2C | 8 | 8 | Adafruit backpacks |
| GPIO | Direct | 7/8/14/16 | 8 | Any raw display |

---

## Supported Platforms

- **AVR** - Arduino Uno, Nano, Mega
- **ESP32** - ESP32 DevKit, etc.
- **STM32** - STM32F1, etc.
- **RP2040** - Raspberry Pi Pico

---

## Installation

### Library Manager

1. Open Arduino IDE
2. Sketch > Include Library > Manage Libraries
3. Search for "XSegment_lib"
4. Click Install

### Manual

1. Download the zip from GitHub
2. Sketch > Include Library > Add .zip Library
3. Select the downloaded file

---

## Quick Start

```cpp
#include <XSegment_lib.h>

// Create display instance (driver, pins, digits)
XSeg_TM1637 display(2, 3, 4);  // CLK, DIO, 4 digits

void setup() {
    display.begin();
    display.show("1234");
}

void loop() {
    display.update();
}
```

---

## API Reference

### Initialization

```cpp
void begin();    // Initialize hardware
void clear();    // Clear display
void update();   // Refresh display (call in loop)
```

### Display Content

```cpp
void show(int32_t n);      // Show integer
void show(uint32_t n);     // Show unsigned integer
void show(float n, 2);    // Show float (specify decimals)
void show("text");        // Show string
```

### Configuration

```cpp
void setBrightness(7);    // 0-7 (or 0-15 for MAX7219/HT16K33)
void setAlignment(XSEG_ALIGN_RIGHT);   // RIGHT, LEFT, CENTER
void setPadding(XSEG_PAD_SPACE);        // SPACE or ZERO
void setBlink(XSEG_BLINK_MEDIUM);       // OFF, SLOW, MEDIUM, FAST
```

---

## Examples

### TM1637 (4-digit clock module)

```cpp
#include <XSegment_lib.h>

XSeg_TM1637 display(2, 3, 4);

void setup() {
    display.begin();
    display.show("1234");
}

void loop() {
    display.update();
}
```

### MAX7219 (8-digit chain)

```cpp
#include <XSegment_lib.h>

XSeg_MAX7219 display(10, 8, 2);  // CS pin, 8 digits, 2 chips

void setup() {
    display.begin();
    display.show("12345678");
}

void loop() {
    display.update();
}
```

### HT16K33 (I2C backpack)

```cpp
#include <XSegment_lib.h>

XSeg_HT16K33 display(0x70, 4);  // I2C address, 4 digits

void setup() {
    display.begin();
    display.show("AB12");
}

void loop() {
    display.update();
}
```

### Raw GPIO (7-segment)

```cpp
#include <XSegment_lib.h>

uint8_t segPins[] = {2,3,4,5,6,7,8};  // a,b,c,d,e,f,g
uint8_t digitPins[] = {9,10,11,12};   // digit 1-4

XSeg_GPIO display(segPins, digitPins, 4, XSEG_TYPE_7, XSEG_COMMON_CATHODE);

void setup() {
    display.begin();
    display.show(1234);
}

void loop() {
    display.update();
}
```

---

## Advanced Features

### Blinking

```cpp
display.setBlink(XSEG_BLINK_MEDIUM);
display.show("1234");
```

### Scrolling Text

```cpp
display.scroll("Hello World!", 300, XSEG_SCROLL_LEFT);
```

### Float Numbers

```cpp
display.show(12.34, 2);  // Show "12.34" (2 decimal places)
```

### Custom Characters

```cpp
display.setDigit(0, XSEG_A | XSEG_B | XSEG_C | XSEG_D | XSEG_E | XSEG_F);  // "A"
```

---

## Building & Testing

### Local Tests (Make)

```bash
# Run unit tests with Make
cd tests_ci
make test

# Run specific test groups
make math      # Math utilities tests
make buffer    # Buffer tests
make clean    # Clean compiled binaries
```

### PlatformIO Tests

```bash
# Run unit tests with PlatformIO (native)
pio test -e native

# Run on ESP32
pio test -e esp32

# Run on Arduino Uno
pio test -e uno
```

### Build for Different Platforms

```bash
# Arduino AVR (Uno, Nano, Mega)
arduino-cli compile --fqbn arduino:avr:uno examples/TM1637/Basic/Basic.ino

# Arduino ESP32
arduino-cli compile --fqbn esp32:esp32:esp32dev examples/TM1637/Basic/Basic.ino

# Arduino STM32
arduino-cli compile --fqbn STM32:stm32:GenF1 examples/TM1637/Basic/Basic.ino

# Raspberry Pi Pico (RP2040)
arduino-cli compile --fqbn rp2040:rp2040:rpipico examples/TM1637/Basic/Basic.ino
```

### Visual Simulation (Wokwi)

Open the JSON files in `tests/simulation/` at [Wokwi.com](https://wokwi.com):
- `wokwi_tm1637.json` - TM1637 driver
- `wokwi_tm1638.json` - TM1638 driver
- `wokwi_max7219.json` - MAX7219 driver
- `wokwi_ht16k33.json` - HT16K33 driver
- `wokwi_gpio_7seg.json` - GPIO 7-segment
- `wokwi_gpio_14seg.json` - GPIO 14-segment
