#include "cgnuino.h"

CgnDI button = CgnDI(2);

void setup() {
  Serial.begin(115200);
}

void loop() {
  button.update();

  if (button.turnon()) {
    Serial.println("Pushed");
  } else if (button.turnoff()) {
    Serial.println("Released");
  }

  delay(1);
}

