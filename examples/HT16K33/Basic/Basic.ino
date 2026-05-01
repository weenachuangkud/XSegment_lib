#include <XSegment_lib.h>

XSeg_HT16K33 display(0x70, 4);

void setup() {
  display.begin();
  display.show("1234");
}

void loop() {
  display.update();
}
