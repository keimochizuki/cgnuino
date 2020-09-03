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
 * @param s Name of the current task period.
 * @param l Maximum length of current task period in [ms].
**/
void CgnPeriod::set(String s, uint32_t l) {
  period = s;
  if (l == 0) {
    limit = ULONG_MAX;
  } else {
    limit = millis() + l;
  }
}

/*!
 * @brief Checks whether the current task period is \a s.
 * @param s Name of the candidate task period.
**/
bool CgnPeriod::is(String s) {
  return period == s;
}

/*!
 * @brief Checks whether the current task period expired its time limitation.
**/
bool CgnPeriod::expire() {
  return millis() > limit;
}

/*!
 * @brief Shows current task period.
**/
String CgnPeriod::get() {
  return period;
}

/*!
 * @brief Shows the time limitation of the current task period.
**/
uint32_t CgnPeriod::until() {
  return limit;
}

