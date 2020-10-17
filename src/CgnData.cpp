/*!
 * @file CgnData.cpp
 * @brief Definition of CgnData class.
 * @author Kei Mochizuki
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Constructor.
 * @param separatingChar Separator for serial outputs (by default @\t).
**/
CgnData::CgnData(char separatingChar) {
  sep = separatingChar;
  data = "";
}

/*!
 * @brief Appends a new text to output buffer.
 * @param newData Appended text data.
**/
void CgnData::append(String newData) {
  data = data + sep + newData;
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

