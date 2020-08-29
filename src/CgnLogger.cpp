/*!
 * @file CgnLogger.cpp
 * @brief Definition of CgnLogger class.
 * @author Kei Mochizuki
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Constructor.
 * @param b Initial value of the logged boolean.
 * @param o First pin number for relaied output pins.
 * @param d Delay intervened after a bit change in [ms].
**/
CgnLogger::CgnLogger(bool b, byte o, byte d) {
  cur = b;
  pre = cur;
  relay = o;
  relaid = (relay != NULL);
  r = d;
  last = millis();
  rest = 0;
  if (relaid) {
    pinMode(relay, OUTPUT);
    digitalWrite(relay, cur ? HIGH : LOW);
  }
}

/*!
 * @brief Updates boolean buffer by current value.
 * @note For a normal usage, this method is intended to be called
 *       once, and only once, inside \c loop function.
**/
void CgnLogger::update(bool b) {
  int past;
  past = millis() - last;
  last = millis();

  if (rest > past) {
    rest -= past;

  } else {
    rest = 0;
    pre = cur;
    cur = b;

    if (cur != pre) {
      rest = r;
      if (relaid) {
        digitalWrite(relay, cur ? HIGH : LOW);
      }
    }
  }
}

/*!
 * @brief Checks whether current value is \c true.
**/
bool CgnLogger::on() {
  return cur;
}

/*!
 * @brief Checks whether current value is \c false.
**/
bool CgnLogger::off() {
  return !cur;
}

/*!
 * @brief Checks whether the buffer turned on in current loop.
**/
bool CgnLogger::turnon() {
  return cur && !pre;
}

/*!
 * @brief Checks whether the buffer turned off in current loop.
**/
bool CgnLogger::turnoff() {
  return !cur && pre;
}

/*!
 * @brief Checks whether the buffer was changed from previous loop.
**/
bool CgnLogger::change() {
  return cur != pre;
}

/*!
 * @brief Checks whether the buffer kept unchanged from previous loop.
**/
bool CgnLogger::keep() {
  return cur == pre;
}

