/*!
 * @file CgnStrobe.cpp
 * @brief Definition of CgnStrobe class.
 * @author Kei Mochizuki
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Constructor.
 * @param p First pin number for digital-out pins.
 * @param l Length of each character in [us] (\b MICRO-seconds).
**/
CgnStrobe::CgnStrobe(byte p, uint32_t l) {
  first = p;
  if (l > 2000) {
    len = l / 1000;
    us = false;
  } else {
    len = l;
    us = true;
  }
  for (int i = 0; i < 9; i++) {
    pinMode(first + i, OUTPUT);
    digitalWrite(first + i, LOW);
  }
}

/*!
 * @brief Emits arbitrary text by (8 + 1)-bit digital outputs.
 * @param s Text to be put out.
**/
uint32_t CgnStrobe::out(String s) {
  bool tmp;
  byte n = s.length();
  uint32_t time;

  time = millis();
  for (int i = 0; i < n; i++) {
    //Serial.print(s.charAt(i));
    //Serial.print(" ");

    for (int j = 7; j >= 0; j--) {
      tmp = (byte(s.charAt(i)) & _BV(j)) > 0;
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

  return millis() - time;
}

void CgnStrobe::wait() {
  if (us) {
    delayMicroseconds(len);
  } else {
    delay(len);
  }
}

