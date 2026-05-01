#include <XSegment_lib.h>

uint8_t segPins[] = {2, 3, 4, 5, 6, 7, 8};
XSeg_GPIO display(segPins, nullptr, 1, XSEG_TYPE_7, XSEG_COMMON_CATHODE);

void setup() {
  display.begin();
  display.show(8);
}

void loop() {
  display.update();
}
