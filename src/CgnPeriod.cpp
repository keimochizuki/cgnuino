/*!
 * @file CgnPeriod.cpp
 * @brief Definition of CgnPeriod class.
 * @author Kei Mochizuki
 * @example Period.ino
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Constructor.
**/
CgnPeriod::CgnPeriod() {
  period = "";
  limit = ULONG_MAX;
}

/*!
 * @brief Sets current task period and its time limitation.
 * @param newPeriod Name of the new task period.
 * @param lengthMs Maximum length of current task period in [ms].
**/
void CgnPeriod::set(String newPeriod, uint32_t lengthMs) {
  period = newPeriod;
  if (lengthMs > (ULONG_MAX - millis())) {
    limit = ULONG_MAX;
  } else {
    limit = millis() + lengthMs;
  }
}

/*!
 * @brief Checks whether the current task period is \a s.
 * @param periodName Name of the candidate task period.
 * @return Whether the current period matches the designated one.
**/
bool CgnPeriod::is(String periodName) {
  return period == periodName;
}

/*!
 * @brief Checks whether the current task period expired its time limitation.
 * @return Whether time limitation of the current period has expired.
**/
bool CgnPeriod::expire() {
  return millis() > limit;
}

/*!
 * @brief Shows current task period.
 * @return Name of the current period.
**/
String CgnPeriod::get() {
  return period;
}

/*!
 * @brief Shows the time limitation of the current task period.
 * @return Time limiation of the current period.
**/
uint32_t CgnPeriod::until() {
  return limit;
}

