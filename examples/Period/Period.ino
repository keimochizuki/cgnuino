#include "cgnuino.h"

CgnStopwatch sw;
CgnPeriod period;

void setup() {
  Serial.begin(115200);
  period.set("rest");
}

void loop() {
  if (period.is("rest")) {
    period.set("resp", 1234);
    Serial.println("resp start");
    sw.lap();

  } else if (period.is("resp") && period.expire()) {
    period.set("iti", 1000);
    Serial.print("resp expire: ");
    Serial.println(sw.get());
    
  } else if (period.is("iti") && period.expire()) {
    period.set("rest", 0);

  }
  delay(1);
}


