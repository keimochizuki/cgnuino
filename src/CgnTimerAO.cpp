/*!
 * @file CgnTimerAO.cpp
 * @brief Definition of CgnTimerAO class.
 * @author Kei Mochizuki
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Consructor.
**/
CgnTimerAO::CgnTimerAO() {
  limit = ULONG_MAX;
}

/*!
 * @brief Changes the analog output to a given value when determined time length has passed.
 * @return Difference between intended and actual timer action in [ms].
 *         \c ULONG_MAX is returned when timer action did not occur.
 * @note For a normal usage, this method is intended to be called
 *       once inside \c loop function.
**/
uint32_t CgnTimerAO::update() {
  uint32_t d = ULONG_MAX;
  if(millis() >= limit) {
    analogWrite(pin, value);
    d = millis() - limit;
    limit = ULONG_MAX;
  }
  return d;
}

/*!
 * @brief Sets a timer to change the analog output after a given time length.
 * @param aoPin Pin number for analog-out.
 * @param timerMs Time length of timer in [ms].
 * @param aoValue Value of the analog output to change to.
**/
void CgnTimerAO::set(byte aoPin, uint32_t timerMs, byte aoValue) {
  pin = aoPin;
  limit = millis() + timerMs;
  value = aoValue;
}

/*!
 * @brief Shows the pin number currently monitored (returns 255 when not in use).
 * @return Pin number of currently scheduled analog output.
**/
byte CgnTimerAO::get() {
  if (limit != ULONG_MAX) {
    return pin;
  } else {
    return 255;
  }
}

/*!
 * @brief Shows the time limitation of the current timer.
 * @return Time limitation of currently scheduled analog output.
**/
uint32_t CgnTimerAO::until() {
  return limit;
}

