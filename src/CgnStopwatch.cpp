/*!
 * @file CgnStopwatch.cpp
 * @brief Definition of CgnStopwatch class.
 * @author Kei Mochizuki
 * @example Lchika.ino
 * @example LchikaWave.ino
 * @example Period.ino
 * @example SeqResp.ino
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
 * @return Time past from last \c lap in [ms].
**/
uint32_t CgnStopwatch::lap() {
  uint32_t tmp;
  tmp = millis() - from;
  from = millis();
  return tmp;
}

/*!
 * @brief Shows elapsed time in [ms] without resarting the clock.
 * @return Time past from last \c lap in [ms].
**/
uint32_t CgnStopwatch::get() {
  return millis() - from;
}


