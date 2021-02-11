#include "cgnuino.h"

CgnPeriod period;

void setup() {
  Serial.begin(115200);
  period.set("rest");
}

void loop() {
  if (period.is("rest")) {
    period.set("pretrial", 500);
    Serial.println("pre-trial");

  } else if (period.is("pretrial") && period.expire()) {
    period.set("trial", 2000);
    Serial.println("trial progressing");

  } else if (period.is("trial") && period.expire()) {
    period.set("iti", 1000);
    Serial.println("inter-trial interval");

  } else if (period.is("iti") && period.expire()) {
    period.set("rest");
    Serial.println("");

  }
  delay(1);
}

