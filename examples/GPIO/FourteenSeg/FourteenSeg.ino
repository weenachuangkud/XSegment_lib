#include <XSegment_lib.h>

uint8_t segPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1};
XSeg_GPIO display(segPins, nullptr, 1, XSEG_TYPE_14, XSEG_COMMON_CATHODE);

void setup() {
  display.begin();
  display.show("A");
}

void loop() {
  display.update();
}
