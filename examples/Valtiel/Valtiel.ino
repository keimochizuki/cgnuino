#include "cgnuino.h"

CgnValtiel val;

void setup() {
  randomSeed(analogRead(A0));
  Serial.begin(115200);
}

void loop() {
  float tmp;
  tmp = val.update();
  if (random(100) == 0) {
    Serial.println(tmp);
  }
  delay(random(1, 20));
}

