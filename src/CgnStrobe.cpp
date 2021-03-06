/*!
 * @file CgnStrobe.cpp
 * @brief Definition of CgnStrobe class.
 * @author Kei Mochizuki
 * @example Strobe.ino
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Constructor.
 * @param firstPin First pin number for digital-out pins.
 * @param strobeUs Length of each character in [us] (\b MICRO-seconds).
 * @param sendTerminator Whether to send terminator character (\c 0x00) at the end.
**/
CgnStrobe::CgnStrobe(byte firstPin, uint32_t strobeUs, bool sendTerminator) {
  first = firstPin;
  term = sendTerminator;
  if (strobeUs > 2000) {
    len = strobeUs / 1000;
    us = false;
  } else {
    len = strobeUs;
    us = true;
  }

  for (int i = 0; i < 9; i++) {
    pinMode(first + i, OUTPUT);
    digitalWrite(first + i, LOW);
  }
}

/*!
 * @brief Emits arbitrary text by (8 + 1)-bit digital outputs.
 * @param txt Text to be put out.
 * @return Time spent by strobing in [ms].
**/
uint32_t CgnStrobe::out(String txt) {
  bool tmp;
  byte n = txt.length();
  uint32_t from = millis();

  for (int i = 0; i < n; i++) {
    //Serial.print(s.charAt(i));
    //Serial.print(" ");

    for (int j = 7; j >= 0; j--) {
      tmp = (byte(txt.charAt(i)) & _BV(j)) > 0;
      digitalWrite(first + j, tmp ? HIGH : LOW);
      //Serial.print(tmp);
    }
    digitalWrite(first + 8, HIGH);
    //Serial.println("");
    wait();
    digitalWrite(first + 8, LOW);
    wait();
  }

  for (int j = 7; j >= 0; j--) {
    digitalWrite(first + j, LOW);
  }
  if (term) {
    digitalWrite(first + 8, HIGH);
    wait();
    digitalWrite(first + 8, LOW);
  }

  return millis() - from;
}

void CgnStrobe::wait() {
  if (us) {
    delayMicroseconds(len);
  } else {
    delay(len);
  }
}

