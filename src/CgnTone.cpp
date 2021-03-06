/*!
 * @file CgnTone.cpp
 * @brief Definition of CgnTone class.
 * @author Kei Mochizuki
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Consructor.
 * @param tonePin Pin number for digital-out to a piezo buzzer.
**/
CgnTone::CgnTone(byte tonePin) {
  pin = tonePin;
  limit = ULONG_MAX;

  pinMode(pin, OUTPUT);
  noTone(pin);
}

/*!
 * @brief Stop the tone when finished determined time length of output.
 * @return Difference between intended and actual tone length in [ms].
 *         \c ULONG_MAX is returned when termination of tone did not occur.
 * @note For a normal usage, this method is intended to be called
 *       once inside \c loop function.
**/
uint32_t CgnTone::update() {
  uint32_t d = ULONG_MAX;
  if(millis() >= limit) {
    noTone(pin);
    d = millis() - limit;
    limit = ULONG_MAX;
  }
  return d;
}

/*!
 * @brief Starts a tone out from a pin for determined time length.
 * @param toneMs Time length of output in [ms].
 * @param toneFreq Frequency of tone output in [Hz].
**/
void CgnTone::out(uint32_t toneMs, uint16_t toneFreq) {
  tone(pin, toneFreq);
  limit = millis() + toneMs;
}

