/*!
 * @file CgnValtiel.cpp
 * @brief Definition of CgnValtiel class.
 * @author Kei Mochizuki
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Constructor.
**/
CgnValtiel::CgnValtiel() {
  n = 0;
  from = millis();
}

/*!
 * @brief Stores the length of the last loop and show averaged loop length.
 * @note For a normal usage, this method is intended to be called
 *       once, and only once, inside \c loop function.
**/
float CgnValtiel::update() {
  float tmp = 0;
  byte m;

  l[n % N_CGNVALTIEL] = millis() - from;
  n += 1;
  from = millis();

  m = min(n, N_CGNVALTIEL);
  for (int i = 0; i < m; i++) {
    tmp += l[i];
  }
  return tmp / (float)m;
}

