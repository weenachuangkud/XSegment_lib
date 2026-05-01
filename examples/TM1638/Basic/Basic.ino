#include <XSegment_lib.h>

XSeg_TM1638 display(2, 3, 4);

void setup() {
  display.begin();
  display.show("ARDUINO");
}

void loop() {
  display.update();
}
