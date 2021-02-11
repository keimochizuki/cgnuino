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
 * @brief Stop the analog output when finished determined time length of output.
 * @note For a normal usage, this method is intended to be called
 *       once inside \c loop function.
**/
void CgnAO::update() {
  if(millis() > limit) {
    analogWrite(pin, 0);
    limit = ULONG_MAX;
  }
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

