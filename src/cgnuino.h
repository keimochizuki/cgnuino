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

constexpr uint32_t ULONG_MAX = 4294967295; //!< Maximal value for unsigned long.
constexpr byte BYTE_MAX = 255; //!< Maximal value for byte.
constexpr byte N_CGNDI = 10; //!< Number of pins that can be simultaneously set for a CgnDI instance.
constexpr byte N_CGNDO = 10; //!< Number of pins that can be simultaneously set for a CgnDO instance.

/*!
 * @brief Communicates with external control apprication running on a secondary PC.
 *
 * In normal psychological experiments in humans,
 * no "manpowered" monitoring of task progression is usually required.
 * Once the task is run for a given participant,
 * values of the variables in the task
 * will change by themselves, i.e., as you programmed in the code.
 * However, especially in animal experiments,
 * there are situations you come to wish for on-line modification
 * of variable without stopping the task.
 * For example, you may want to change the time length of
 * certain task period or parameter, depending on the animal's
 * temporary motivational state.
 * You may also want to prepare more or less different
 * task modes in a single Arduino Sketch,
 * and manually switch them pro re nata.
 *
 * Although this is somewhat a difficult request for
 * "compiled language" like Arduino,
 * you can achieve it by sending command through a serial connection
 * and receiving it via CgnControl class.
 * CgnControl reads incoming serial text from Arduino's
 * standard \c Serial using \c readStringUntil method.
 * The received text is assumed to be composed of
 * two elements, "code" and "value", separated by a colon
 * (e.g., 1:2000, 2:3.14, or 5:trainingMode).
 * A "code" is an integer which precede the colon,
 * and can be used for conditional branching in your Sketch
 * (detailed example usage will be presented below).
 * A "value" is an arbitrary text that succeeds the colon,
 * and is the value you want to pass to Arduino.
 * CgnControl decompose received text into these two elements
 * by spliting it at intervened colon.
 * By accessing these elements by \c getCode and \c getValue methods,
 * you can prepare arbitrary conditional branches in your Sketch.
 *
 * \code
 * CgnControl ctrl;
 * int x;
 * float y;
 * String z;
 * 
 * ctrl.update();
 * switch (ctrl.getCode()) {
 *   case 1:
 *     x = ctrl.getValue().toInt();
 *     break;
 *   case 2:
 *     y = ctrl.getValue().toFloat();
 *     break;
 *   case 5:
 *     z = ctrl.getValue();
 *     break;
 * }
 * \endcode
 *
 * In the example above, you can dynamically change
 * the value of \c x to \c 2000 by sending 1:2000 through a serial.
 * Similarly, sending 2:3.14 will set \c y as \c 3.14,
 * and 5:trainingMode set \c z as \c "trainingMode".
 * As this example shows, how the received value
 * is used in conditional branches is completely up to you.
 * In other words, the received value is simply left in String type,
 * so you need to put appropriate type casting and
 * explicitly determine which variable should store that value
 * in each \c case branches.
 *
 * If received text is composed only one character,
 * CgnControl regards it as an ascii-coded integer.
 * This allows you an easy conditional control on task's behavior
 * by one-character serial emittion.
 * For example, on receiving a text "A", CgnControl regards
 * it as a "code" of \c 65 (ascii code of upper "A").
 * Samely sending "a" is received as a "code" of \c 97.
 * Therefore, just typing "A" and "a",
 * you can turn on and off the 13th GPIO pin in the example code below.
 *
 * \code
 * ctrl.update();
 * switch (ctrl.getCode()) {
 *   case 65:
 *     digitalWrite(13, HIGH);
 *     break;
 *   case 97:
 *     digitalWrite(13, LOW);
 *     break;
 * }
 * \endcode
 *
 * As this example clearly shows, you don't need to restrict
 * the usage of CgnControl for variable modification.
 * In fact, it offers an flexible mechanism with which
 * you can perform arbitrary on-line conditional branching
 * through serial interaction.
**/
class CgnControl {
  public:
    CgnControl(char = 10);
    String update();
    int getCode();
    String getValue();

  private:
    int c;
    String v;
    char eol;
};

/*!
 * @brief Offers convenient digital-in buffering.
 *
 * Digital-in pins make Arduino boards detect signal changes
 * in outer world as high (5V [or 3.3V]) and low (0V) voltage.
 * The standard way to record these signals by Arduino is
 * to use \c digitalRead function, which returns
 * \c HIGH or \c LOW (special constants in Arduino) depending on
 * the incoming signal.
 *
 * In constructing behavioral tasks, this function can be used
 * to monitor any arbitrary electrical parts or instruments
 * that put out information about their state as a
 * 5V/0V digital (binary) signal.
 * One of the most common cases is to record participant's
 * action as button press, lever manipulation and so on.
 * For example, if you connect a mechanical switch with
 * either pull-up or pull-down resistor, you can monitor
 * the button's push/release state as \c HIGH or \c LOW
 * (depending on whether you pulled up or down the pin).
 * Especially in digital inputs on Arduino boards,
 * there is a convenient pin mode called \c INPUT_PULLUP.
 * This feature sets the designated pin as input mode,
 * and internally connects it to high votage
 * with inbuilt registor about 10k ohm (i.e., pulls up the designated pin).
 * Thus, by inserting a simple single-pole switch to this pin and GND,
 * you can record the state of the switch as \c HIGH (released) or
 * \c LOW (pressed) without any external pull-up mechanism.
 *
 * \code
 * byte pin = 13;
 * bool isReleased;
 * pinMode(pin, INPUT_PULLUP);
 * isReleased = digitalRead(pin);
 * \endcode
 *
 * However, in behavioral tasks, there are often times that
 * you want to know whether the button is pressed and released
 * at this moment (in this loop).
 * In other words, you may want to detect,
 * not only the button's current state,
 * but also the timing of changes of the button press/release.
 * In this case, you need to buffer the previous value from the pin,
 * and compare it with the current pin state read by \c digitalRead function.
 * Furthermore, there can be multiple of such input like
 * home, center, left and right buttons, etc...
 * These conditions force you to deal with an array of input buffers
 * and repeated comparison processes every time you want to
 * refer to the current, latest values of these inputs.
 *
 * CgnDI class offers an easy way for such input monitoring
 * in behavioral tasks.
 * At construction, CgnDI class prepares multiple pins as
 * pulled-up digital inputs using \c INPUT_PULLUP.
 * Maximal number of pins for one instance of this class
 * is determined by a constant \c N_CGNDI.
 * Every time \c update is called, CgnDI class read the
 * current values of the input pins.
 * These values can be checked by \c on and \c off methods,
 * which returns \c true when the button is pressed
 * (i.e., the pin is connected to GND by the switch),
 * and \c false when it is not pressed
 * (i.e., the pin is connected to 5V through pull-up registor).
 * In other words, original negative logic values
 * (\c LOW/HIGH for switch's on/off, respectively)
 * read by \c digitalRead function are
 * converted to \c true/false boolean with a translation
 * to a positive logic.
 * Also CgnDI class remembers the previous values of the pins in a buffer.
 * Thus you can check if the value was changed at the time of
 * last \c update call by just performing \c turnon and \c turnoff methods.
 * Because of this operation mechanism, \c update method
 * is intended to be performed once (and normally only once)
 * in every loop which is repeated in reasonablly quick cycles
 * (usually a few milliseconds will be enough,
 * but it depends on situation of each experiment).
 *
 * CgnDI class relays the current values of digital inputs
 * to other set of digital-out pins,
 * when dedicated pin numbers for this facility is designated.
 * This can be used to relay the buttons' on/off state
 * to other measurement instruments.
 * CgnDI class also has a so called debounce mechanism.
 * This is a (in the case of cgnuino library) software mechanism
 * that conquer the short, unstable input fluctuation
 * at the very moment of switch opening and closing.
 * High flequent alteration of high/low voltage,
 * called chattering or ripple, is problematic
 * since Arduino may misregard it as multiple numbers of
 * (really really) quick turing on and off of the switch.
 * To prevent it, CgnDI class omits input to a digital-in pin
 * right after once a change occured (by default a few milliseconds),
 * discarding possible chattering input during this period.
 * Although absolute amount of time this silencing takes place
 * is universal to all the pins for a given CgnDI instance,
 * actual omission is performed separately for each pin.
 * In other words, if a change has occured to a given pin,
 * succeeding input to that same pin is omitted for a short period,
 * but inputs to other pins are regularly received and accessible.
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
 *
 * Digital-out pins make Arduino boards emit digital signal as
 * high (5V [or 3.3V]) or low (0V) voltage.
**/
class CgnDO {
  public:
    CgnDO(byte, byte = 1);
    void update();
    void out(byte, uint32_t);

  private:
    byte first;
    byte n;
    uint32_t limit[N_CGNDO];
};

/*!
 * @brief Stores trial information and print it to the Serial.
**/
class CgnData {
  public:
    CgnData(char = 9);
    void append(String);
    void out();
    void clear();

  private:
    char sep;
    String data;
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
 * @brief Emits asynchroneous tone output.
**/
class CgnTone {
  public:
    CgnTone(byte);
    void update();
    void out(uint32_t, uint16_t = 440);

  private:
    byte pin;
    uint32_t limit;
};

/*!
 * @brief Monitors average length of executed loops on Arduino.
**/
class CgnValtiel {
  public:
    CgnValtiel();
    void start();
    float lap();
    uint32_t getMax();
    uint32_t getMin();

  private:
	uint32_t from;
	uint32_t last;
	uint32_t n;
	uint32_t mx;
	uint32_t mn;
};

#endif

