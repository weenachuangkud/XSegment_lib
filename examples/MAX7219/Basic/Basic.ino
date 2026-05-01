#include <XSegment_lib.h>

// Use hardware SPI by leaving clk/din as default (-1)
XSeg_MAX7219 display(10, 4, 1);

void setup() {
  display.begin();
  display.show("1234");
}

void loop() {
  display.update();
}
