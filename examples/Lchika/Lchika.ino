#include "cgnuino.h"

CgnStopwatch sw;
CgnDO led = CgnDO(13);

void setup() {
  Serial.begin(115200);
}

void loop() {
  led.update();
  if (sw.get() > 1000) {
    sw.lap();
    led.out(0, 500);
  }
  delay(1);
}

