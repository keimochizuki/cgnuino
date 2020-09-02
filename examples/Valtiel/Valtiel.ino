#include "cgnuino.h"

CgnValtiel val;
byte n = 0;

void setup() {
  val.start();
  randomSeed(analogRead(A0));
  Serial.begin(115200);
}

void loop() {
  n += 1;
  if (n >= 100) {
    Serial.println(val.lap());
    val.start();
    n = 0;
  } else {
    val.lap();
  }
  delay(random(1, 20));
}

