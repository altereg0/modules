/*
 *   Automaton.h - Reactive State Machine Framework for Arduino.
 *     Published under the MIT License (MIT), Copyright (c) 2015-2016, J.P. van der Landen
 *     */

#pragma once


class atm_counter {
 public:
  uint16_t value;
  void set( uint16_t v );
  uint8_t expired( void );
  uint16_t decrement( void );
};
