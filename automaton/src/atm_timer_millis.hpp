/*
 *   Automaton - Reactive State Machine Framework for Arduino.
 *   Published under the MIT License (MIT), Copyright (c) 2015-2016, J.P. van der Landen
 */

#ifndef ATM_TIMER_MILLIS_H_
#define ATM_TIMER_MILLIS_H_

typedef uint32_t atm_timer_millis_t;

class atm_timer_millis {
 public:
  atm_timer_millis_t value;
  void set( atm_timer_millis_t v );
  void setFromNow( Machine* machine, atm_timer_millis_t v );
  int expired( Machine* machine );
};

#endif