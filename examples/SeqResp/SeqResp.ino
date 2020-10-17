#include "cgnuino.h"

CgnStopwatch sw;
CgnDI swtch = CgnDI(2, 1, 13);
byte n = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  swtch.update();
  if (swtch.turnon()) {
    Serial.print(sw.lap());
    n += 1;
    if (n < 3) {
      Serial.print(" - ");
    } else {
      n = 0;
      Serial.println("");
    }
  }
  delay(1);
}

