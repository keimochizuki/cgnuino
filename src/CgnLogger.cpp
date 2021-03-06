/*!
 * @file CgnLogger.cpp
 * @brief Definition of CgnLogger class.
 * @author Kei Mochizuki
 * @example Logger.ino
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Constructor.
 * @param initBool Initial value of the logged boolean.
 * @param relaidPin Pin number for relaied output pin.
 * @param debounceMs Delay intervened after a bit change in [ms].
**/
CgnLogger::CgnLogger(bool initBool, byte relaidPin, byte debounceMs) {
  cur = initBool;
  pre = cur;
  relay = relaidPin;
  relaid = (relay != NULL);
  r = debounceMs;
  last = millis();
  rest = 0;

  if (relaid) {
    pinMode(relay, OUTPUT);
    digitalWrite(relay, cur ? HIGH : LOW);
  }
}

/*!
 * @brief Updates boolean buffer by current value.
 * @param newBool New value of the logged boolean.
 * @return Time separation between current and last \c update in [ms].
 * @note For a normal usage, this method is intended to be called
 *       once, and only once, inside \c loop function.
**/
uint32_t CgnLogger::update(bool newBool) {
  int past;
  past = millis() - last;
  last = millis();

  pre = cur;
  if (rest > past) {
    rest -= past;

  } else {
    rest = 0;
    cur = newBool;

    if (cur != pre) {
      rest = r;
      if (relaid) {
        digitalWrite(relay, cur ? HIGH : LOW);
      }
    }
  }
  return past;
}

/*!
 * @brief Checks whether current value is \c true.
 * @return Result of the examined boolean state.
**/
bool CgnLogger::on() {
  return cur;
}

/*!
 * @brief Checks whether current value is \c false.
 * @return Result of the examined boolean state.
**/
bool CgnLogger::off() {
  return !cur;
}

/*!
 * @brief Checks whether the buffer turned on in current loop.
 * @return Result of the examined boolean state.
**/
bool CgnLogger::turnon() {
  return cur && !pre;
}

/*!
 * @brief Checks whether the buffer turned off in current loop.
 * @return Result of the examined boolean state.
**/
bool CgnLogger::turnoff() {
  return !cur && pre;
}

/*!
 * @brief Checks whether the buffer was changed from previous loop.
 * @return Result of the examined boolean state.
**/
bool CgnLogger::change() {
  return cur != pre;
}

/*!
 * @brief Checks whether the buffer kept unchanged from previous loop.
 * @return Result of the examined boolean state.
**/
bool CgnLogger::keep() {
  return cur == pre;
}

