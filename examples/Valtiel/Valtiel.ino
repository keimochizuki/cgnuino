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
  if (n >= 20) {
    Serial.print("average: ");
    Serial.print(val.lap());
    Serial.print(", max: ");
    Serial.print(val.getMax());
    Serial.print(", min: ");
    Serial.print(val.getMin());
    Serial.println(" [ms/loop]");

    val.start();
    n = 0;
  } else {
    val.lap();
  }
  delay(random(1, 100));
}

