/*!
 * @file CgnData.cpp
 * @brief Definition of CgnData class.
 * @author Kei Mochizuki
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Constructor.
 * @param s Separator for serial outputs (by default @\t).
**/
CgnData::CgnData(char s) {
  sep = s;
  data = "";
}

/*!
 * @brief Appends a new text to output buffer.
 * @param s Appended text.
**/
void CgnData::append(String s) {
  data = data + sep + s;
}

/*!
 * @brief Emits buffered text to serial output.
**/
void CgnData::out() {
  Serial.println(data);
  data = "";
}

/*!
 * @brief Clears existing output buffer.
**/
void CgnData::clear() {
  data = "";
}

