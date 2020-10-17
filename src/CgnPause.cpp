/*!
 * @file CgnPause.cpp
 * @brief Definition of CgnPause class.
 * @author Kei Mochizuki
 * @example Pause.ino
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Constructor.
 * @param pausePin Pin number of digital input to invoke pausing.
 * @param stopWhen Pin value when the task should pause.
 * @param pauseMs Checking cycle for the task to restart in [ms].
**/
CgnPause::CgnPause(byte pausePin, bool stopWhen, uint16_t pauseMs) {
  pin = pausePin;
  b = stopWhen;
  cycle = pauseMs;
  pinMode(pin, INPUT_PULLUP);
}

/*!
 * @brief Checks the designated digital-in pin for pausing.
**/
void CgnPause::check() {
  while(digitalRead(pin) == b) {
    delay(cycle);
  }
}

