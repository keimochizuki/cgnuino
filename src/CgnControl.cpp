/*!
 * @file CgnControl.cpp
 * @brief Definition of CgnControl class.
 * @author Kei Mochizuki
 * @example Control.ino
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Constructor.
 * @param e EOL for serial inputs (by default @\n).
**/
CgnControl::CgnControl(char e) {
  c = 0;
  v = "";
  eol = e;
}

/*!
 * @brief Checks the serial buffer for a new input line.
**/
String CgnControl::update() {
  int i;
  String s;

  c = 0;
  v = "";
  if (Serial.available() > 0) {
    s = Serial.readStringUntil(eol);
    s.trim();
    if (s.length() > 0) {
      if (s.length() == 1) {
        // for one-character command
        c = int(s[0]);
      } else {
        i = s.indexOf(":");
        if (i == -1) {
          // when separater did not exist
          v = s;
        } else {
          // for online variable modulation
          v = s.substring(i + 1);
          v.trim();
          s = s.substring(0, i); // this does not include i-th char
          s.trim();
          c = s.toInt();
        }
      }
    }
  }
  return v;
}

/*!
 * @brief Shows decomposed code for the last serial input.
**/
int CgnControl::getCode() {
  return c;
}

/*!
 * @brief Shows decomposed value for the last serial input.
**/
String CgnControl::getValue() {
  return v;
}

