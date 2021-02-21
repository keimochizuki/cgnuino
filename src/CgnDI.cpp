/*!
 * @file CgnDI.cpp
 * @brief Definition of CgnDI class.
 * @author Kei Mochizuki
 * @example DI.ino
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Constructor.
 * @param firstPin First pin number for digital-in pins.
 * @param numberOfInputs Number of digital inputs in use.
 * @param relaidPin First pin number for relaied output pins if needed.
 * @param debounceMs Delay intervened after a bit change in [ms].
**/
CgnDI::CgnDI(byte firstPin, byte numberOfInputs, byte relaidPin, byte debounceMs) {
  first = firstPin;
  n = numberOfInputs;
  relay = relaidPin;
  relaid = (relay != NULL);
  r = debounceMs;
  last = millis();

  for (int i = 0; i < N_CGNDI; i++) {
    rest[i] = 0;
    if (i < n) {
      pinMode(first + i, INPUT_PULLUP);
      cur[i] = digitalRead(first + i) ? false : true;
      pre[i] = cur[i];

      if (relaid) {
        pinMode(relay + i, OUTPUT);
        digitalWrite(relay + i, cur[i] ? HIGH : LOW);
      }
    } else {
      cur[i] = false;
      pre[i] = false;
    }
  }
}

/*!
 * @brief Updates DI buffer by current pin voltages.
 * @return Time separation between current and last \c update in [ms].
 * @note For a normal usage, this method is intended to be called
 *       once, and only once, inside \c loop function.
**/
uint32_t CgnDI::update() {
  uint32_t past;
  past = millis() - last;
  last = millis();

  for (int i = 0; i < n; i++) {
    pre[i] = cur[i];
    if (rest[i] > past) {
      rest[i] -= past;
      continue;

    } else {
      rest[i] = 0;
      cur[i] = digitalRead(first + i) ? false : true;

      if (cur[i] != pre[i]) {
        rest[i] = r;
        if (relaid) {
          digitalWrite(relay + i, cur[i] ? HIGH : LOW);
        }
      }
    }
  }
  return past;
}

/*!
 * @brief Checks whether @a i-th DI pin is on (active).
 * @param i Index of input you want to check.
 * @return Result of the examined pin state.
**/
bool CgnDI::on(byte i) {
  return cur[i];
}

/*!
 * @brief Checks whether @a i-th DI pin is off (inactive).
 * @param i Index of input you want to check.
 * @return Result of the examined pin state.
**/
bool CgnDI::off(byte i) {
  return !cur[i];
}

/*!
 * @brief Checks whether @a i-th DI pin was turned on in current loop.
 * @param i Index of input you want to check.
 * @return Result of the examined pin state.
**/
bool CgnDI::turnon(byte i) {
  return cur[i] && !pre[i];
}

/*!
 * @brief Checks whether @a i-th DI pin was turned off in current loop.
 * @param i Index of input you want to check.
 * @return Result of the examined pin state.
**/
bool CgnDI::turnoff(byte i) {
  return !cur[i] && pre[i];
}

/*!
 * @brief Checks whether @a i-th DI pin was changed from previous loop.
 * @param i Index of input you want to check.
 * @return Result of the examined pin state.
**/
bool CgnDI::change(byte i) {
  return cur[i] != pre[i];
}

/*!
 * @brief Checks whether @a i-th DI pin kept unchanged from previous loop.
 * @param i Index of input you want to check.
 * @return Result of the examined pin state.
**/
bool CgnDI::keep(byte i) {
  return cur[i] == pre[i];
}

