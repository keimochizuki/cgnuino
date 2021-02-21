/*!
 * @file CgnTimerDO.cpp
 * @brief Definition of CgnTimerDO class.
 * @author Kei Mochizuki
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Consructor.
**/
CgnTimerDO::CgnTimerDO() {
  limit = ULONG_MAX;
}

/*!
 * @brief Changes the digital output to a given value when determined time length has passed.
 * @return Difference between intended and actual timer action in [ms].
 *         \c ULONG_MAX is returned when timer action did not occur.
 * @note For a normal usage, this method is intended to be called
 *       once inside \c loop function.
**/
uint32_t CgnTimerDO::update() {
  uint32_t d = ULONG_MAX;
  if(millis() >= limit) {
    digitalWrite(pin, value);
    d = millis() - limit;
    limit = ULONG_MAX;
  }
  return d;
}

/*!
 * @brief Sets a timer to change the digital output after a given time length.
 * @param doPin Pin number for digital-out.
 * @param timerMs Time length of timer in [ms].
 * @param doValue Value of the digital output to change to.
**/
void CgnTimerDO::set(byte doPin, uint32_t timerMs, bool doValue) {
  pin = doPin;
  limit = millis() + timerMs;
  value = doValue;
}

/*!
 * @brief Shows the pin number currently monitored (returns 255 when not in use).
 * @return Pin number of currently scheduled digital output.
**/
byte CgnTimerDO::get() {
  if (limit != ULONG_MAX) {
    return pin;
  } else {
    return 255;
  }
}

/*!
 * @brief Shows the time limitation of the current timer.
 * @return Time limitation of currently scheduled digital output.
**/
uint32_t CgnTimerDO::until() {
  return limit;
}

