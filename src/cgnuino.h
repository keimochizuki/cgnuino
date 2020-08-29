/*!
 * @file cgnuino.h
 * @brief Header file for cgnuino library.
 * @author Kei Mochizuki
**/

#ifndef INCLUDED_CGNUINO
#define INCLUDED_CGNUINO

#include "Arduino.h"

/*!
 * @def countof
 * @brief Replaced by the length of the array when compiled.
**/
#define countof(array) (sizeof(array) / sizeof(array[0]))

extern const uint32_t ULONG_MAX; //!< Maximal value for unsigned long.
extern const byte BYTE_MAX; //!< Maximal value for byte.
constexpr byte N_CGNDI = 10; //!< Number of pins that can be simultaneously set for a CgnDI instance.
constexpr byte N_CGNDO = 10; //!< Number of pins that can be simultaneously set for a CgnDO instance.
constexpr byte N_CGNVALTIEL = 20; //!< Number of buffers CgnValtiel, speed checker for main loop, can holds.

/*!
 * @brief Offers convenient digital-in buffering.
**/
class CgnDI {
  public:
    CgnDI(byte, byte = 1, byte = NULL, byte = 2);
    void update();
    bool on(byte = 0);
    bool off(byte = 0);
    bool turnon(byte = 0);
    bool turnoff(byte = 0);
    bool change(byte = 0);
    bool keep(byte = 0);

  private:
    byte first;
    byte n;
    byte relay;
    bool relaid;
    bool cur[N_CGNDI];
    bool pre[N_CGNDI];
    byte r;
    byte rest[N_CGNDI];
    uint32_t last;
};

/*!
 * @brief Emits asynchroneous digital-out.
**/
class CgnDO {
  public:
    CgnDO(byte, byte = 1, char = 'd');
    void update();
    void out(byte, uint32_t, uint16_t = 440);

  private:
    byte first;
    byte n;
    char type;
    uint32_t limit[N_CGNDO];
};

/*!
 * @brief Logs arbitorary bit change similar to CgnDI class.
**/
class CgnLogger {
  public:
    CgnLogger(bool = false, byte = NULL, byte = 0);
    void update(bool);
    bool on();
    bool off();
    bool turnon();
    bool turnoff();
    bool change();
    bool keep();

  private:
    bool cur;
    bool pre;
    byte relay;
    bool relaid;
    byte r;
    byte rest;
    uint32_t last;
};

/*!
 * @brief Temporally pauses task progression by digital-in pin state.
**/
class CgnPause {
  public:
    CgnPause(byte, bool = LOW, uint16_t = 100);
    void check();

  private:
    byte pin;
    bool stopwhen;
    uint16_t cycle;
};

/*!
 * @brief Remembers current task period and its time constraint.
**/
class CgnPeriod {
  public:
    CgnPeriod();
    void set(String, uint32_t = 0);
    bool is(String);
    bool expire();
    String get();
    uint32_t until();

  private:
    String period;
    uint32_t limit;
};

/*!
 * @brief Communicates with external control apprication running on the PC.
**/
class CgnSerial {
  public:
    CgnSerial(char = 10, char = 9);
    String update(bool = true);
    int getCode();
    String getValue();
    void append(String);
    void out();
    void clear();

  private:
    int c;
    String v;
    char eol;
    char sep;
    String data;
};

/*!
 * @brief Measures time difference in milliseconds.
**/
class CgnStopwatch {
  public:
    CgnStopwatch();
    uint32_t lap();
    uint32_t get();

  private:
    uint32_t from;
};

/*!
 * @brief Emits a text as one-by-one characters using (8 + 1)-bit digital-out.
**/
class CgnStrobe {
  public:
    CgnStrobe(byte, uint32_t = 5);
    uint32_t out(String);

  private:
    void wait();
    byte first;
	uint32_t len;
    bool us;
};

/*!
 * @brief Monitors average length of executed loops on Arduino.
**/
class CgnValtiel {
  public:
    CgnValtiel();
    float update();

  private:
	uint32_t from;
	uint32_t n;
    byte l[N_CGNVALTIEL];
};

#endif

