#include "cgnuino.h"

CgnPause pause = CgnPause(13);
byte n = 0, inc = 1;

void setup() {
  Serial.begin(115200);
}

void loop() {
  byte tmp = n;

  pause.check();
  while (tmp > 0) {
    Serial.print(" ");
    tmp -= 1;
  }
  Serial.println("*");

  n += inc;
  if (n == 10) {
    inc = -1;
  } else if (n == 0) {
    inc = 1;
  }

  delay(250);
}

