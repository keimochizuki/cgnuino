/*!
 * @file CgnSerial.cpp
 * @brief Definition of CgnSerial class.
 * @author Kei Mochizuki
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Constructor.
 * @param e EOL for serial inputs (by default @\n).
 * @param s Separator for serial outputs (by default @\t).
**/
CgnSerial::CgnSerial(char e, char s) {
  c = 0;
  v = "";
  eol = e;
  sep = s;
  data = "";
}

/*!
 * @brief Checks the serial buffer for a new input line.
 * @param decompose Whether to decompose the input text (see Details of the class).
**/
String CgnSerial::update(bool decompose) {
  int i;
  String s;

  c = 0;
  v = "";
  if (Serial.available() > 0) {
    s = Serial.readStringUntil(eol);
    s.trim();
    if (s.length() > 0) {
      if (!decompose) {
        // for raw string input
        v = s;
      } else if (s.length() == 1) {
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
int CgnSerial::getCode() {
  return c;
}

/*!
 * @brief Shows decomposed value for the last serial input.
**/
String CgnSerial::getValue() {
  return v;
}

/*!
 * @brief Appends a new value to output buffer.
 * @param s Appended string.
**/
void CgnSerial::append(String s) {
  if (data != "") {
    data += sep;
  }
  data += s;
}

/*!
 * @brief Emits buffered text to serial output.
**/
void CgnSerial::out() {
  Serial.println(data);
  data = "";
}

/*!
 * @brief Clears existing output buffer.
**/
void CgnSerial::clear() {
  data = "";
}

