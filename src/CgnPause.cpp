/*!
 * @file CgnPause.cpp
 * @brief Definition of CgnPause class.
 * @author Kei Mochizuki
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Constructor.
 * @param p Pin number of digital input.
 * @param b Pin value when the task should pause.
 * @param c Checking cycle for the task to restart in [ms].
**/
CgnPause::CgnPause(byte p, bool b, uint16_t c) {
  pin = p;
  stopwhen = b;
  cycle = c;
  pinMode(pin, INPUT_PULLUP);
}

/*!
 * @brief Checks the designated digital-in pin for pausing.
**/
void CgnPause::check() {
  while(digitalRead(pin) == stopwhen) {
    delay(cycle);
  }
}

