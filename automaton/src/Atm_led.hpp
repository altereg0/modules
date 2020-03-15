#ifndef AUTOMATON_LED_H_
#define AUTOMATON_LED_H_

#include <Automaton.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

// WMath prototypes
long random(long);
long random(long, long);
void randomSeed(unsigned long);
long map(long, long, long, long, long);

template<board::DigitalPin attached_pin, bool activeLow>
class Atm_led : public Machine {
 public:
  // clang-format off
  enum { IDLE, ON, START, BLINK_OFF, LOOP, DONE, OFF, WT_ON, WT_START };
  enum { EVT_ON_TIMER, EVT_OFF_TIMER, EVT_WT_TIMER, EVT_COUNTER, EVT_ON, EVT_OFF, EVT_BLINK, EVT_TOGGLE, EVT_TOGGLE_BLINK, ELSE, EVT_BRUP, EVT_BRDN }; // BRUP/BRDN pseudo
  enum { EVT_START = EVT_BLINK };
  // clang-format on

  Atm_led(void) : Machine(), led{gpio::PinMode::OUTPUT} {};

  Atm_led &begin(void) {
    // clang-format off
    static const state_t state_table[] PROGMEM = {
        /*               ON_ENTER    ON_LOOP    ON_EXIT  EVT_ON_TIMER  EVT_OFF_TIMER EVT_WT_TIMER EVT_COUNTER  EVT_ON  EVT_OFF  EVT_BLINK  EVT_TOGGLE  EVT_TOGGLE_BLINK   ELSE */
        /* IDLE      */  ENT_INIT, ATM_SLEEP,        -1,           -1,            -1,          -1,         -1,  WT_ON,      -1,  WT_START,         ON,         WT_START,    -1, // LED off
        /* ON        */    ENT_ON, ATM_SLEEP,        -1,           -1,            -1,          -1,         -1,     -1,     OFF,  WT_START,        OFF,              OFF,    -1, // LED on
        /* START     */    ENT_ON,        -1,        -1,    BLINK_OFF,            -1,          -1,         -1,  WT_ON,     OFF,        -1,        OFF,              OFF,    -1, // Start blinking
        /* BLINK_OFF */   ENT_OFF,        -1,        -1,           -1,          LOOP,          -1,         -1,  WT_ON,     OFF,        -1,        OFF,              OFF,    -1,
        /* LOOP      */        -1,        -1,        -1,           -1,            -1,          -1,       DONE,  WT_ON,     OFF,        -1,        OFF,              OFF, START,
        /* DONE      */        -1,        -1, EXT_CHAIN,           -1,           OFF,          -1,         -1,  WT_ON,     OFF,  WT_START,        OFF,              OFF,    -1, // Wait after last blink
        /* OFF       */   ENT_OFF,        -1,        -1,           -1,            -1,          -1,         -1,  WT_ON,     OFF,  WT_START,         -1,               -1,  IDLE, // All off -> IDLE
        /* WT_ON     */        -1,        -1,        -1,           -1,            -1,          ON,         -1,  WT_ON,     OFF,  WT_START,         -1,               -1,    -1, // LEAD for ON
        /* WT_START  */        -1,        -1,        -1,           -1,            -1,       START,         -1,  WT_ON,     OFF,  WT_START,         -1,               -1,    -1, // LEAD for BLINK
    };
  // clang-format on
    Machine::begin(state_table, ELSE);
    level = 255;
    toLow = 0;
    toHigh = 255;
    wrap = false;
    initLED();
    on_timer.set(500);
    off_timer.set(500);
    pwm(512, 1);
    lead_timer.set(0);
    repeat_count = ATM_COUNTER_OFF;
    counter.set(repeat_count);
    while (state() != 0) cycle();
    return *this;
  };
  Atm_led &blink(void) {
    trigger(EVT_BLINK);
    return *this;
  };
  Atm_led &blink(atm_timer_millis_t duration) {
    on_timer.set(duration);  // Time in which led is fully on
    return *this;
  };
  Atm_led &blink(atm_timer_millis_t duration,
                 atm_timer_millis_t pause_duration,
                 uint16_t repeat_count = ATM_COUNTER_OFF) {
    blink(duration);  // Time in which led is fully on
    pause(pause_duration);
    repeat(repeat_count);
    return *this;
  };
  Atm_led &pwm(uint16_t width_, float freq_ = -1) {
    if (freq > -1) {
      this->freq = freq_;
    } else {
      freq_ = this->freq;
    }
    this->width = width_;
    float cycle_width = 1000 / freq_;
    on_timer.set(cycle_width / 1024 * this->width);
    off_timer.set(cycle_width / 1024 * (1024 - this->width));
    return *this;
  };
  Atm_led &frequency(float freq_) {
    this->freq = freq_;
    float cycle_width = 1000 / freq_;
    on_timer.set(cycle_width / 1024 * this->width);
    off_timer.set(cycle_width / 1024 * (1024 - this->width));
    return *this;
  };
  Atm_led &pause(uint32_t duration) {  // Time in which led is fully off
    off_timer.set(duration ? duration : 1);     // Make sure off_timer is never 0 (work around)
    return *this;
  };
  Atm_led &fade(int fade) {
    return *this;
  };  // Dummy for method compatibility with Atm_fade
  Atm_led &lead(uint32_t ms) {
    lead_timer.set(ms);
    return *this;
  };
  Atm_led &repeat(uint16_t repeat) {
    counter.set(repeat_count = repeat);
    return *this;
  };
  int brightness(int level = -1) {
    if (level > -1) {
      this->level = level;
      if (current == ON || current == START) {
        setBrightness(mapLevel(level));
      }
    }
    return this->level;
  };
  Atm_led &on(void) {
    trigger(EVT_ON);
    return *this;
  };
  Atm_led &off(void) {
    trigger(EVT_OFF);
    return *this;
  };
  Atm_led &toggle(void) {
    trigger(EVT_TOGGLE);
    return *this;
  };
  Atm_led &toggleBlink(void) {
    trigger(EVT_TOGGLE_BLINK);
    return *this;
  };
  Atm_led &start(void) {
    trigger(EVT_BLINK);
    return *this;
  };
  Atm_led &trace(Stream &stream) {
    setTrace(&stream, atm_serial_debug::trace,
             "LED\0EVT_ON_TIMER\0EVT_OFF_TIMER\0EVT_WT_TIMER\0EVT_COUNTER\0EVT_ON\0EVT_OFF\0EVT_"
             "BLINK\0EVT_TOGGLE\0EVT_TOGGLE_BLINK\0ELSE\0"
             "IDLE\0ON\0START\0BLINK_OFF\0LOOP\0DONE\0OFF\0WT_ON\0WT_START");
    return *this;
  };
  Atm_led &onFinish(Machine &machine, int event = 0) {
    onfinish.set(&machine, event);
    return *this;
  };
  Atm_led &onFinish(atm_cb_push_t callback, int idx = 0) {
    onfinish.set(callback, idx);
    return *this;
  };
  Atm_led &range(int toLow, int toHigh, bool wrap = false) {
    this->toLow = toLow;
    this->toHigh = toHigh;
    this->wrap = wrap;
    level = toHigh;
    return *this;
  };
  Atm_led &levels(unsigned char *map, int mapsize, bool wrap = false) {
    this->levelMap = map;
    levelMapSize = mapsize;
    range(0, mapsize - 1, wrap);
    return *this;
  };
  int brighten(int v = 1) {
    if (abs(v) == 1) {
      int br = (int) this->level + v;
      if (br > toHigh)
        br = wrap ? toLow : toHigh;
      if (br < toLow)
        br = wrap ? toHigh : toLow;
      brightness(br);
    }
    return this->level;
  };
  Atm_led &trigger(int event) {
    if (event > ELSE) {
      brighten(event == EVT_BRUP ? 1 : -1);
    } else {
      Machine::trigger(event);
    }
    return *this;
  };

 private:
  enum { ENT_INIT, ENT_ON, ENT_OFF, EXT_CHAIN };
  uint8_t level;
  uint16_t repeat_count;
  uint16_t width;
  gpio::FAST_PIN<attached_pin> led;
  float freq;
  atm_timer_millis on_timer, off_timer, lead_timer;
  atm_counter counter;
  atm_connector onfinish;
  unsigned char *levelMap;
  int levelMapSize;

  int event(int id) {
    switch (id) {
      case EVT_ON_TIMER:return on_timer.expired(this);
      case EVT_OFF_TIMER:return off_timer.expired(this);
      case EVT_WT_TIMER:return lead_timer.expired(this);
      case EVT_COUNTER:return counter.expired();
    }
    return 0;
  };
  void action(int id) {
    switch (id) {
      case ENT_INIT:counter.set(repeat_count);
        return;
      case ENT_ON:switchOn();
        return;
      case ENT_OFF:counter.decrement();
        switchOff();
        return;
      case EXT_CHAIN:onfinish.push(0);
        return;
    }
  };
 protected:
  uint8_t toHigh, toLow;
  bool wrap;
  virtual void initLED() {
//	pinMode(pin, OUTPUT);
//	digitalWrite(pin, activeLow ? HIGH : LOW);
//  initPwmTimer1();
    if (activeLow) led.set();
  };
  virtual void switchOn() {
    // Never turn if on_timer is zero (duty cycle 0 must be dark)
    if (on_timer.value == 0) return;
    if (activeLow) {
//		digitalWrite(pin, LOW);
      led.clear();
    } else {
      if (level == toHigh) {
//			digitalWrite(pin, HIGH);
        led.set();
      } else {
//			analogWrite(pin, mapLevel(level));
//            writeGpioPinPwmV(pin, mapLevel(level));
//TODO: FIX FAST
      }
    }
  };
  virtual void switchOff() {
    if (!activeLow) {
//    digitalWrite(pin, LOW);
      led.clear();
    } else {
      if (level == toHigh) {
//      digitalWrite(pin, HIGH);
        led.set();
      } else {
//      analogWrite(pin, mapLevel(level));
//      writeGpioPinPwmV(pin, mapLevel(level));
//TODO: FIX FAST
        led.clear();
      }
    }
  };
  virtual void setBrightness(UNUSED int value) {
//	analogWrite( pin, value );
//  writeGpioPinPwmV(pin, value);
//TODO: FIX FAST
  };
  virtual int mapLevel(int level) {
    if (levelMapSize) {
      return levelMap[level];
    } else {
      return map(level, toLow, toHigh, 0, 255);
    }
  };
};

#endif