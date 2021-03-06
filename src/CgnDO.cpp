/*!
 * @file CgnDO.cpp
 * @brief Definition of CgnDO class.
 * @author Kei Mochizuki
 * @example Lchika.ino
 * @example LchikaWave.ino
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Consructor.
 * @param firstPin First pin number for digital-out pins.
 * @param numberOfOutputs Number of digital outputs in use.
**/
CgnDO::CgnDO(byte firstPin, byte numberOfOutputs) {
  first = firstPin;
  n = numberOfOutputs;

  for (int i = 0; i < N_CGNDO; i++) {
    limit[i] = ULONG_MAX;
    if (i < n) {
      pinMode(first + i, OUTPUT);
      digitalWrite(first + i, LOW);
    }
  }
}

/*!
 * @brief Lowers down the pins that finished determined time length of output.
 * @return Difference between intended and actual output lengths in [ms].
 *         \c ULONG_MAX is returned when termination of output did not occur.
 * @note For a normal usage, this method is intended to be called
 *       once inside \c loop function.
**/
uint32_t CgnDO::update() {
  uint32_t d = ULONG_MAX, cur = millis();
  for (int i = 0; i < n; i++) {
    if(cur >= limit[i]) {
      digitalWrite(first + i, LOW);
      d = cur - limit[i];
      limit[i] = ULONG_MAX;
    }
  }
  return d;
}

/*!
 * @brief Starts putting out from a pin for determined time length.
 * @param i Index of DO pin to emit digital output.
 * @param outputMs Time length of output in [ms].
**/
void CgnDO::out(byte i, uint32_t outputMs) {
  digitalWrite(first + i, HIGH);
  limit[i] = millis() + outputMs;
}

