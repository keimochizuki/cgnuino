#include "cgnuino.h"
#define LED 13

CgnControl ctrl;
int time_light = 100;
int time_dark = 900;
bool on = false;

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.begin(115200);
}

void loop() {
  ctrl.update();
  switch (ctrl.getCode()) {
    case 1:
      time_light = ctrl.getValue().toInt();
      break;
    case 2:
      time_dark = ctrl.getValue().toInt();
      break;
    case 112:
      while (true) {
        ctrl.update();
        if (ctrl.getCode() == 112) {
          break;
        } else {
          delay(100);
        }
      }
      break;
  }

  if (on) {
    digitalWrite(LED, LOW);
    delay(time_dark);
  } else {
    digitalWrite(LED, HIGH);
    delay(time_light);
  }
  on = !on;
}


