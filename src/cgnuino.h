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
 * task modes in a single Arduino sketch,
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
 * and can be used for conditional branching in your sketch
 * (detailed example usage will be presented below).
 * A "value" is an arbitrary text that succeeds the colon,
 * and is the value you want to pass to Arduino.
 * CgnControl decompose received text into these two elements
 * by spliting it at intervened colon.
 * By accessing these elements by \c getCode and \c getValue methods,
 * you can prepare arbitrary conditional branches in your sketch.
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
 * Therefore, just typing "A" and "a" (and then Enter to send it if needed),
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
 * @brief Emits asynchroneous analog-out in a similar way to CgnDO class.
 *
 * In order to asynchroneously emit digital outputs,
 * cgnuino has CgnDO class dedicated to that purpose.
 * The same demand for an analog output instead of digital outputs
 * can be met by CgnAO class.
 * The usage is almost identical to CgnDO class except that
 * CgnAO class only use one out pin,
 * instead of holding multiple analog-out pins.
 * This is because GPIO pins with pulse width modulation functionality
 * (virtual analog-out function in Arduino)
 * are limited and implemented intermittently on the board,
 * making the perservation of multiple analog-out pins impossible.
 * Start analog output of a given length and strength by \c out method.
 * Then repeatedly call \c update method so that it can terminate
 * the output once a designated time length has passed.
**/
class CgnAO {
  public:
    CgnAO(byte);
    void update();
    void out(uint32_t, byte = 255);

  private:
    byte pin;
    uint32_t limit;
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
 * To prevent it, CgnDI class omits input from a digital-in pin
 * right after once a change occured (by default a few milliseconds),
 * discarding possible chattering input during this period.
 * Although absolute amount of time this silencing takes place
 * is universal to all the pins for a given CgnDI instance,
 * actual omission is performed separately for each pin.
 * In other words, if a change has occured to a given pin,
 * succeeding input from that same pin is omitted for a short period,
 * but inputs from other pins are regularly received and accessible.
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
 * Digital-out pins make Arduino boards emit digital signal,
 * i.e., high (5V [or 3.3V]) or low (0V) voltage.
 * These pins are normally turned on and off
 * via Arduino's built-in \c digitalWrite function.
 * For example, if you want to flash an LED connected to
 * a given pin for a second,
 * you can achieve it with a simple code as below.
 *
 * \code
 * byte pin = 13;
 * pinMode(pin, OUTPUT);
 * digitalWrite(pin, HIGH);
 * delay(1000);
 * digitalWrite(pin, LOW);
 * \endcode
 *
 * Since \c delay function sleeps all the execution of
 * your board for a given length (designated in millisecond),
 * the LED will be extinguished 1000 ms after it is lighted.
 * This way you can activate arbitrary external devices
 * for any time length you want.
 * However, as mentioned above,
 * when you use \c delay function,
 * all the performance of your Arduino board is suspended
 * until the designated time length has passed.
 * This is problematic in controlling behavioral tasks.
 * There are times that
 * you want to turn on a given digital-out pin for a certain length,
 * while you keep continuing other processes
 * (e.g., monitoring subject's button press through digital-in pins).
 * In other words, you will miss subject's button press and release
 * while you are suspending your board using \c delay function.
 *
 * CgnDO class provides an easy way to overcome this problem.
 * At construction, CgnDO class prepares multiple pins as digital outputs.
 * Maximal number of pins for one instance of this class
 * is determined by a constant \c N_CGNDO.
 * To put out high digital output, use \c out method
 * by designating both the index of the digital-out pin
 * (but in count from the predetermined first output pin
 * instead of the absolute pin number of the board)
 * and the time length of the output in millisecond.
 * This puts up the designated digital-out pin to high voltage.
 * Then you need to repeatedly call \c update method.
 * This method checks if the required time has passed,
 * and if so, turns off the pin to low voltage.
 * In normal, this monitoring is achieved by calling
 * \c update method in the beginning of your main \c loop function.
 *
 * Because of the functional mechanism mentioned above,
 * CgnDO class is accurate and makes sense only when
 * your \c loop is repeated in a reasonably frequent cycle.
 * For example, if your \c loop is run every 5 ms on average,
 * the length of digital output from CgnDO class
 * is accurate with maximal 5 ms deviation,
 * which would be acceptable in standard psychological tasks.
 * Note that you can easily monitor the frequency of your \c loop
 * by using CgnValtiel class.
 * Also note that outputs from CgnDO class can be simultaneously done
 * from multiple pins with respectively different time lengths.
 * LchikaWave example will provide a simple example of this usage
 * of CgnDO class.
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
 *
 * CgnData class is a simple string container for temporally
 * storing task information of a current trial.
 * It concatenates strings provided by \c append method
 * to its internal data string, inserting a predetermined
 * separater character (by default @\t).
 * So you can temporally store trial conditions,
 * response times, correct/error flags, or whatever information
 * about current trial.
 * When a trial ended, you can print the resultant data string
 * to regular Serial using \c out method.
 * If a trial is aborted during a progression and
 * the data is no more needed, you can clear the
 * temporal storage by \c clear method.
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
 * @brief Logs arbitrary boolean change in a similar way to CgnDI class.
 *
 * CgnLogger class provides an easy way to log and monitor
 * changes of arbitrary boolean values
 * in a similar way to CgnDI class.
 * In case of CgnDI class, current states (high/low values)
 * of digital-in pins are collected,
 * while storing the previous values.
 * This way not only the present values but also
 * the changes of the pin states can be detected.
 * CgnLogger class does the same thing to any boolean value
 * that is provided once in each loop.
 * You need to provide a boolean value to CgnLogger class
 * by using \c update method at the beginning of each loop in normal.
 * Then you can use the same methods to CgnDI class
 * to check the value changes.
 *
 * For example, there are cases that you want to first
 * get a value from an analog-in pin,
 * judge the obtained value on your own arbitrary criteria,
 * then monitor the changes of resulting boolean (true/false).
 * This cannot be directly done by CgnDI class since
 * the input value is not digitized.
 * Thus you need CgnLogger class to utilize the same
 * facility to CgnDI class in the case
 * that a boolean variable is the target of tracking.
 *
 * Like CgnDI, CgnLogger class also has a debounce mechanism
 * that prevents secondary value change shortly after
 * once a change has occured.
 * Since CgnLogger class does not deal with actual digital inputs
 * (which can have occasional mechanical noizes),
 * debouncing is essentially nonsense for this class.
 * This is why the length of debounce is set to \c 0 by default.
 * However, it is possible that the logged boolean value is
 * judged from some noizy signal (like the usage of analog-in pin
 * in the example above), which can cause quick and repetitive
 * true/false alternation at the time of changes.
 * In such cases, you can use debouncing to prevent
 * restless value changes also in CgnLogger class.
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
 *
 * During an experiment, there may be an occasion that
 * you want to temporally stop the progression of the task.
 * Normally, this is not common in human experiments
 * in which the task should keep progressing until
 * a predetermined number of trials are correctly finished.
 * However, especially in animal experiments,
 * you will often need to pause the task during an experiment.
 * For example, you may not want the task to start
 * before you finish booting all the measurement apparatus
 * even if the animal is ready.
 * In another case, you may need to halt the trial progression
 * until you finish dealing with accidental failure of some device.
 *
 * CgnPause class provides a physical switching mechanism
 * to temporally pause your task.
 * It judges whether to proceed or not based on
 * the high/low state of the designated digital-in pin.
 * The pin is automatically pulled up at the time of
 * construction of CgnPause instance.
 * If the pin is connected to GND, CgnPause class
 * halts the task until the pin becomes unconnected,
 * i.e., re-connected to the internal pull-up.
 * (This relationship can be inverted if you wish to.)
 *
 * If you want to pause the task at any timing during a trial,
 * put \c check method on a main code of \c loop function
 * so that the evaluation of pausing can occur in each loop.
 * However, if the task is suddenly paused within a trial,
 * it is possible that some output pins are stuck
 * at undesired state
 * (e.g., high-locked digital-out pin that keeps driving external device,
 * tone output that keeps beeping during the pause).
 * To prevent this, use \c check method in a task period
 * dedicated for the pausing evaluation during an inter-trial interval.
**/
class CgnPause {
  public:
    CgnPause(byte, bool = LOW, uint16_t = 100);
    void check();

  private:
    byte pin;
    bool b;
    uint16_t cycle;
};

/*!
 * @brief Remembers current task period and its time constraint.
 *
 * A trial in a behavioral task is composed of
 * multiple task periods (also called as epochs),
 * which are temporal intervals within which
 * a set of task events or desired participant's actions are taken place.
 * For example, an LED flash is provided in the cue period,
 * and the participant should press a button as soon as possible
 * in the subsequent response period.
 * Some task periods have time limitation, while others can last
 * forever until predetermined conditions are fulfilled.
 *
 * To manage the transition of numbers of task periods,
 * CgnPeriod class offers a handy cutoff to you.
 * It simply remembers the current task period designated
 * by \c set method.
 * By using CgnPeriod class, conditional branching
 * in your main code will become more readable.
 * If you designate time length of the period on calling \c set method,
 * CgnPeriod class also maintain that information
 * so that you can monitor the expiration of the predetermined time limit
 * using \c expire method.
 *
 * There can be an occasion that you want to unlimitedly wait for
 * some event to occur during a given task period
 * (e.g., a button press during an inter-trial interval
 * before beginning the next trial).
 * In this case, you can set the length of the period to \c -1.
 * Since the length of the period is defined as an unsigned long,
 * setting it to \c -1 cycles its value to the maximal value
 * of unsigned long (i.e., 4294967295 ms).
 * This corresponds to approximately 50 days,
 * virtually meaning that the period lasts forever without time limitation.
 * (Actually this is the default behavior of the \c set method
 * when you omit designing the second argument.)
**/
class CgnPeriod {
  public:
    CgnPeriod();
    void set(String, uint32_t = -1);
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
 *
 * In behavioral tasks, it is incredibly common
 * for you to count elapsed time from a given origin.
 * Being self-explanatory from its name,
 * CgnStopwatch class works as a stopwatch for that faculty.
 * After construction, call \c lap method to
 * obtain the time length from the current origin,
 * and then resetting the clock
 * (i.e., set current time point as a new origin).
 * Use \c get method to check the clock while maintaining
 * the present origin.
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
 *
 * CgnStrobe is a rather peculiar facility
 * compared with other classes in this library.
 * It converts arbitrary length of text to 8-bit digital outputs,
 * and emits it by strobing the other one digital-out pin.
 * This can be used to emit event timing signal
 * in a good time resolution, maintaining the differentiation
 * of multiple event identities.
 *
 * In implementing behavioral tasks with Arduino,
 * experimental data are supposed to be saved
 * to a text file on the PC through a USB serial.
 * In serial communication, emitted texts are designed to be
 * first buffered and wait for a short period
 * to be actually sent.
 * This delay is typically less than several milliseconds,
 * and possibly dozens of milliseconds even at maximum.
 * Still a serial is regarded as unreliable
 * for strict event timing outputs for this buffering problem.
 *
 * To put out strict timing signal, you can use a mere
 * digital outputs from Arduino.
 * The rising/falling edge of digital signal can be captured
 * by external data acquisition device practically without any delay.
 * Although this method is a classic in sending event timing ,
 * it is somewhat unsophisticated since this kind of digital outputs
 * cannot hold information other than the timing.
 * In other words, a single line of digital signal
 * cannot tell what event is taken place.
 * For example, if there are 10 different events in your task
 * (e.g., trial start, cue on, cue off, response start, button press, etc...)
 * you need 10 digital-out pins to mark the occurrences of these
 * possible events individually.
 *
 * To overcome these weak points, you can combine
 * event timing marks by a digital output
 * and multi-bit binary representation of characters.
 * For example, you can express 256 different combination
 * of 0/1 bit pattern using eight digital pins.
 * Which bit pattern corresponds to what character
 * is actually universal for standard English characters
 * as Ascii table (http://www.asciitable.com/).
 *
 * | + | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
 * |:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
 * | 30|   |   |   | ! | @"| @#| @$| @%| @&| ' |
 * | 40| ( | ) | * | + | , | - | @.| / | 0 | 1 |
 * | 50| 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | : | ; |
 * | 60| < | = | > | ? | @@| A | B | C | D | E |
 * | 70| F | G | H | I | J | K | L | M | N | O |
 * | 80| P | Q | R | S | T | U | V | W | X | Y |
 * | 90| Z | [ | \ | ] | ^ | _ | ` | a | b | c |
 * |100| d | e | f | g | h | i | j | k | l | m |
 * |110| n | o | p | q | r | s | t | u | v | w |
 * |120| x | y | z | { |   | } | ~ |   |   |   |
 *
 * Each row corresponds tens place and column ones place.
 * As shown in the table, standard English characters are
 * represented by values ranging from 0 to 127.
 * For example, upper "A" corresponds to 65,
 * and lower "a" corresponds to 97.
 * The value 97 is expressed as 1100001 in binary representation
 * (i.e., 64 + 32 + 1).
 * Therefore, you can express a letter "a" by pulling up
 * the 8th, 7th and 1st bits of the 8-bit set of digital-out pins.
 * However, changing all the eight digital-out pins simultaneously,
 * accurately in microsecond order, is somewhat an uncertain operation.
 * This is why you need the last 9th bit for precise temporal timing.
 * You first change the eight digital-out pins to represent
 * an arbitrary character, and when ready, pull up the 9th pin.
 * This works as a strobe light in photography, and enables
 * temporally accurate transmission of a character.
 * And by repeating 8-bit change and 9th bit rise
 * (and then fall, of course),
 * you can send any length of text you want,
 * but only one-by-one at a time.
 *
 * Some commercial data acquisition devices have
 * receiver mechanism of this character representation.
 * (I personally created this class to use it with
 * Power1401 device from CED.)
 * By using CgnStrobe class with those kind of devices,
 * you can transmit texts easily and temporally more accurate way
 * compared with a USB serial connection.
**/
class CgnStrobe {
  public:
    CgnStrobe(byte, uint32_t = 5, bool = false);
    uint32_t out(String);

  private:
    void wait();
    byte first;
	uint32_t len;
    bool us;
    bool term;
};

/*!
 * @brief Changes a analog output after a given time length has passed.
 *
 * Asynchroneous analog output control is
 * easily carried out by cgnuino's CgnAO class.
 * However, there may be a case that you occasionally
 * want to directly control some analog-out pin as
 * Arduino's default analogWrite function,
 * but with some time offset.
 * CgnTimerAO class offers such low-level pin control
 * for an analog-out pin.
 * Once set a pin number, time offset,
 * and a value to change to (ranging from 0 to 255),
 * repeatedly call \c update method so that
 * CgnTimerAO class can change
 * analog output value after a designated time length has passed.
 *
 * Note that unlike CgnAO class, CgnTimerAO class does not
 * stick to a particular pin, nor change the pin mode
 * on its construction.
 * You need to designate which pin to set the timer
 * every time you call \c set method.
 * This is for the convenience that you can use one instance of
 * CgnTimerAO class for multiple purpose,
 * watching out different analog-out pins here and there.
 * However, this class can monitor only one pin at a time.
 * So if you want multiple timers to control multiple analog-out pins
 * with different time offsets, you need to create two or more
 * instances of CgnTimerAO class.
**/
class CgnTimerAO {
  public:
    CgnTimerAO();
    void update();
    void set(byte, uint32_t, byte);
    byte get();
    uint32_t until();

  private:
    byte pin;
    uint32_t limit;
    byte value;
};

/*!
 * @brief Changes a digital output after a given time length has passed.
 *
 * Asynchroneous digital output control is
 * easily carried out by cgnuino's CgnDO class.
 * However, there may be a case that you occasionally
 * want to directly control some digital-out pin as
 * Arduino's default digitalWrite function,
 * but with some time offset.
 * CgnTimerDO class offers such low-level pin control
 * for a digital-out pin.
 * Once set a pin number, time offset,
 * and a value to change to (\c HIGH/LOW),
 * repeatedly call \c update method so that
 * CgnTimerDO class can change
 * digital output value after a designated time length has passed.
 *
 * Note that unlike CgnDO class, CgnTimerDO class does not
 * stick to a particular pins, nor change the pin modes
 * on its construction.
 * You need to designate which pin to set the timer
 * every time you call \c set method.
 * This is for the convenience that you can use one instance of
 * CgnTimerDO class for multiple purpose,
 * watching out different digital-out pins here and there.
 * However, this class can monitor only one pin at a time.
 * So if you want multiple timers to control multiple digital-out pins
 * with different time offsets, you need to create two or more
 * instances of CgnTimerDO class.
**/
class CgnTimerDO {
  public:
    CgnTimerDO();
    void update();
    void set(byte, uint32_t, bool);
    byte get();
    uint32_t until();

  private:
    byte pin;
    uint32_t limit;
    bool value;
};

/*!
 * @brief Emits asynchroneous tone output in a similar way to CgnDO class.
 *
 * In order to asynchroneously emit digital outputs,
 * cgnuino has CgnDO class dedicated to that purpose.
 * The same demand for tone output instead of digital outputs
 * can be met by CgnTone class.
 * The usage is almost identical to CgnDO class except that
 * CgnTone class only use one out pin,
 * instead of holding multiple digital-out pins.
 * (Normally multiple tone outputs are not required
 * in standard behavioral tasks.)
 * Start beeping a piezo buzzer in a required frequency
 * by \c out method.
 * Then repeatedly call \c update method so that it can terminate
 * tone output once a designated time length has passed.
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
 *
 * When prototyping your experiment,
 * there are often times that you want to check
 * whether your task is runnning in a reasonably good time resolution.
 * Since some of the classes in cgnuino library is dependent
 * on the frequency of \c loop function,
 * it is important to correctly know how quick your \c loop
 * is executed.
 * CgnValtiel class offers an easy way for this.
 * After you \c start the monitoring,
 * call \c lap method once (and only once) in every \c loop call.
 * (For example, put \c lap on the very beginning of your \c loop function.)
 * CgnValtiel class counts the number of laps and the elapsed time
 * from \c start, and tell you the average length of your \c loop
 * (in millisecond).
 * You can also check the maximal and minimal length of your \c loop
 * by \c getMax and \c getMin method.
 * These can be useful since there is a case that
 * a loop gets unexpectedly overlong or overshort in
 * only a realy realy small probability,
 * thus leaving the average length superficially unaffected.
 *
 * CgnValtiel class is named for an imaginary creature
 * which appears on a video game series "Silent Hill".
 * Despite its dreadful appearance, it is actually not a monster
 * but a servant of God of indigenous religion in Silent Hill
 * (thus being a position of an angel).
 * It governs metempsychosis or reincarnation,
 * watching the tide of fates of living things.
 * During a gameplay, it often appears with a revolving valve that
 * symbolizes its religious role in reincarnation.
 * In a deep respect for the Silent Hill series,
 * CgnValtiel class was named for,
 * for its role of monitoring the cycle of your experimental loops.
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

