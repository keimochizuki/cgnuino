#include "cgnuino.h"

CgnLogger l = CgnLogger();
long n;

void randomwalk(long s) {
  n = random(-s, s);
  l.update(n > 0);
}

void setup() {
  randomSeed(analogRead(A0));
  randomwalk(5);

  Serial.begin(115200);
}

void loop() {
  randomwalk(2);

  Serial.print(n);
  if (l.turnon() || l.turnoff()) {
    Serial.println(" : Crossed zero");
  } else {
    Serial.print(", ");
  }

  delay(500);
}

