#include "cgnuino.h"

CgnStopwatch sw;
CgnDO led = CgnDO(8, 4);

void setup() {
  Serial.begin(115200);
}

void loop() {
  led.update();
  if (sw.get() > 1000) {
    sw.lap();
    for (int i = 0; i < 4; i++) {
      led.out(i, 200 * (i + 1));
    }
  }
  delay(1);
}

