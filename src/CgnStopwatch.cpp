/*!
 * @file CgnStopwatch.cpp
 * @brief Definition of CgnStopwatch class.
 * @author Kei Mochizuki
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Constructor.
**/
CgnStopwatch::CgnStopwatch() {
  from = millis();
}

/*!
 * @brief Shows elapsed time in [ms] and resart the clock.
**/
uint32_t CgnStopwatch::lap() {
  uint32_t tmp;
  tmp = millis() - from;
  from = millis();
  return tmp;
}

/*!
 * @brief Shows elapsed time in [ms] without resarting the clock.
**/
uint32_t CgnStopwatch::get() {
  return millis() - from;
}


