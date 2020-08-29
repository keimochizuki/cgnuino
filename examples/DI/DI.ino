#include "cgnuino.h"

CgnDI button = CgnDI(13);

void setup() {
  Serial.begin(115200);
}

void loop() {
  button.update();

  if (button.turnon(0)) {
    Serial.println("Pushed");
  } else if (button.turnoff(0)) {
    Serial.println("Released");
  }

  delay(1);
}

