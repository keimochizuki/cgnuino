/*!
 * @file CgnDO.cpp
 * @brief Definition of CgnDO class.
 * @author Kei Mochizuki
**/

#include "Arduino.h"
#include "cgnuino.h"

/*!
 * @brief Consructor.
 * @param p First pin number for digital-out pins.
 * @param s Number of digital-out pins.
 * @param t Type of output (digital or tone).
**/
CgnDO::CgnDO(byte p, byte s, char t) {
  first = p;
  n = s;
  type = t;
  for (int i = 0; i < N_CGNDO; i++) {
    limit[i] = ULONG_MAX;
    if (i < n) {
      pinMode(first + i, OUTPUT);
      digitalWrite(first + i, LOW);
    }
  }
}

/*!
 * @brief Lowers down the pins that finished determined time length of output.
 * @note For a normal usage, this method is intended to be called
 *       once inside \c loop function.
**/
void CgnDO::update() {
  uint32_t cur = millis();
  for (int i = 0; i < n; i++) {
    if(cur > limit[i]) {
      if (type == 't') {
        noTone(first + i);
      } else {
        digitalWrite(first + i, LOW);
      }
      limit[i] = ULONG_MAX;
    }
  }
}

/*!
 * @brief Starts putting out from a pin for determined time length.
 * @param i Index of DO pin to emit digital-out.
 * @param l Time length of output in [ms].
 * @param f Frequency of tone output in [Hz].
**/
void CgnDO::out(byte i, uint32_t l, uint16_t f) {
  if (type == "t") {
    tone(first + i, f);
  } else {
    digitalWrite(first + i, HIGH);
  }
  limit[i] = millis() + l;
}

