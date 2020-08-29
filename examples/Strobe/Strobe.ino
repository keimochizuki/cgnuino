#include "cgnuino.h"

CgnStrobe strobe = CgnStrobe(4);

void setup() {
  Serial.begin(115200);
}

void loop() {
  uint32_t t;
  t = strobe.out("Testing word strobe!");
  Serial.print("took ");
  Serial.print(t);
  Serial.println(" ms");

  delay(10000);
}


