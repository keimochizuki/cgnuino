/*!
 * @file CgnValtiel.cpp
 * @brief Definition of CgnValtiel class.
 * @author Kei Mochizuki
 * @example Valtiel.ino
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Constructor.
**/
CgnValtiel::CgnValtiel() {
  from = millis();
  last = from;
  n = 0;
  mx = 0;
  mn = ULONG_MAX;
}

/*!
 * @brief Start monitoring loop length by resetting the counter.
**/
void CgnValtiel::start() {
  from = millis();
  last = from;
  n = 0;
  mx = 0;
  mn = ULONG_MAX;
}

/*!
 * @brief Show average length of past loops and count up the counter.
 * @note For a normal usage, this method is intended to be called
 *       once, and only once, inside \c loop function.
**/
float CgnValtiel::lap() {
  uint32_t cur;
  cur = millis();

  n += 1;
  mx = max(mx, cur - last);
  mn = min(mn, cur - last);
  last = cur;
  return (float)(cur - from) / (float)n;
}

/*!
 * @brief Show maximal length of past loop in [ms].
**/
uint32_t CgnValtiel::getMax() {
  return mx;
}

/*!
 * @brief Show minimal length of past loop in [ms].
**/
uint32_t CgnValtiel::getMin() {
  return mn;
}

