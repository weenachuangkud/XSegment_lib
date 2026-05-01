#include <XSegment_lib.h>

XSeg_TM1637 display(2, 3, 4);

void setup() {
  display.begin();
  display.show("1234");
}

void loop() {
  display.update();
}
