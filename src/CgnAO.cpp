/*!
 * @file CgnAO.cpp
 * @brief Definition of CgnAO class.
 * @author Kei Mochizuki
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Consructor.
 * @param aoPin Pin number for analog output (must be a pin with PWM function).
**/
CgnAO::CgnAO(byte aoPin) {
  pin = aoPin;
  limit = ULONG_MAX;

  pinMode(pin, OUTPUT);
  analogWrite(pin, 0);
}

/*!
 * @brief Stops the analog output when finished determined time length of output.
 * @return Difference between intended and actual output lengths in [ms].
 *         \c ULONG_MAX is returned when termination of output did not occur.
 * @note For a normal usage, this method is intended to be called
 *       once inside \c loop function.
**/
uint32_t CgnAO::update() {
  uint32_t d = ULONG_MAX;
  if(millis() >= limit) {
    analogWrite(pin, 0);
    d = millis() - limit;
    limit = ULONG_MAX;
  }
  return d;
}

/*!
 * @brief Starts an analog output from a pin for determined time length.
 * @param aoMs Time length of output in [ms].
 * @param aoDuty Duty rate of pwm output within a range of [0, 255].
**/
void CgnAO::out(uint32_t aoMs, byte aoDuty) {
  analogWrite(pin, aoDuty);
  limit = millis() + aoMs;
}

