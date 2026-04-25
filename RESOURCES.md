## TM1637
- Datasheet: https://www.mcielectronics.cl/website_MCI/static/documents/Datasheet_TM1637.pdf
- Protocol timing reference: https://github.com/avishorp/TM1637

## TM1638
- Datasheet: https://www.rlocman.ru/i/File/dat/TME/TM1638.pdf
- Protocol reference: https://github.com/gavinlyonsrepo/TM1638plus

---

## MAX7219
- Official datasheet: https://www.analog.com/media/en/technical-documentation/data-sheets/MAX7219-MAX7221.pdf
- SPI protocol reference: https://github.com/wayoda/LedControl

---

## HT16K33
- Official datasheet: https://www.holtek.com/documents/10179/116711/HT16K33v120.pdf
- I2C reference: https://github.com/adafruit/Adafruit_LED_Backpack

---

## AVR Registers
- AVR Libc reference: https://www.nongnu.org/avr-libc/user-manual/group__avr__io.html
- PORT manipulation: https://www.arduino.cc/en/Reference/PortManipulation

---

## ESP32 Registers
- ESP32 GPIO driver: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html
- ESP32 Technical reference: https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf

---

## STM32 Registers
- STM32 HAL reference: https://www.st.com/resource/en/user_manual/um1725-description-of-stm32f4-hal-and-lowlayer-drivers-stmicroelectronics.pdf
- BSRR register: https://www.st.com/resource/en/reference_manual/rm0090-stm32f405415-stm32f407417-stm32f427437-and-stm32f429439-advanced-armbased-32bit-mcus-stmicroelectronics.pdf

---

## RP2040 Registers
- RP2040 datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf
- SIO hardware registers: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf#tab-registerlist_sio

---

## 7 Segment font bitmasks
- Segment layout reference: https://en.wikipedia.org/wiki/Seven-segment_display
- Character encoding reference: https://en.wikipedia.org/wiki/Seven-segment_display_character_encodings

---

## 14 Segment font bitmasks
- Segment layout reference: https://en.wikipedia.org/wiki/Fourteen-segment_display
- ASCII font reference: https://github.com/dmadison/LED-Segment-ASCII

---

## 16 Segment font bitmasks
- Segment layout reference: https://en.wikipedia.org/wiki/Sixteen-segment_display
- Character encoding reference: https://www.keshikan.net/fonts-e.html

---

## Arduino library format
- Library specification: https://arduino.github.io/arduino-cli/0.35/library-specification
- keywords.txt format: https://arduino.github.io/arduino-cli/0.35/library-specification/#keywords
- library.properties format: https://arduino.github.io/arduino-cli/0.35/library-specification/#libraryproperties

---

**PROGMEM AVR**
- PROGMEM reference: https://www.arduino.cc/reference/en/language/variables/utilities/progmem
- pgmspace.h reference: https://www.nongnu.org/avr-libc/user-manual/group__avr__pgmspace.html

---

**Things worth double checking**

| Area | What to verify |
|---|---|
| TM1637 timing | `_delayUs` values — some modules need longer delays |
| MAX7219 scan limit | `_numDigits - 1` register value |
| HT16K33 colon register | Position `0x04` — varies by module |
| Font bitmasks | Cross check against Wikipedia encoding tables |
| 14/16 seg bitmasks | Verify diagonal segment bit positions |
| AVR PROGMEM read | `pgm_read_byte` vs `pgm_read_word` per type |
| ESP32 GPIO high pins | `GPIO.out_w1ts` only works for pins 0-31 |
| STM32 BSRR | Upper 16 bits for reset, lower for set |
