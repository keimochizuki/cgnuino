#include "cgnuino.h"

CgnStrobe strobe = CgnStrobe(4, 50000);

void setup() {
  Serial.begin(115200);
}

void loop() {
  uint32_t t;
  t = strobe.out("Testing word strobe!");
  Serial.print("took ");
  Serial.print(t);
  Serial.println(" ms");

  delay(5000);
}


