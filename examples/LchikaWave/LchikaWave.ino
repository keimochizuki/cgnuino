#include "cgnuino.h"

CgnStopwatch sw;
CgnDO led = CgnDO(8, 4);

void setup() {
  Serial.begin(115200);
}

void loop() {
  led.update();
  if (sw.get() > 2000) {
    sw.lap();
    led.out(0, 500);
    led.out(1, 600);
    led.out(2, 1000);
    led.out(3, 1500);
  }
  delay(1);
}

