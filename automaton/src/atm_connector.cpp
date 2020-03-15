/*
  Automaton.cpp - Reactive State Machine Framework for Arduino.
  Published under the MIT License (MIT), Copyright (c) 2015-2016, J.P. van der Landen
*/

#include "Automaton.h"

/*
 * The atm_connector class facilitates creating push and pull connections between
 * State Machines.
 *
 *********************************************************************************************
 *
 * push( v, up, overrideCallback ) - Calls a machine's trigger method or a callback
 *
 * Will return false if a callback is configured while the overrideCallback arg was specified
 */

bool atm_connector::push( int v /* = 0 */, int up /* = 0 */, bool overrideCallback /* = false */ ) {
  switch ( mode_flags & 0x07 ) { //0b00000111
    case MODE_PUSHCB:
      if ( overrideCallback ) {
        return false;
      } else {
        if ( extra_.cb_callback_.cb_type_.push_callback ) {
          ( *extra_.cb_callback_.cb_type_.push_callback )( extra_.cb_callback_.callback_idx, v, up );
        }
      }
      return true;
    case MODE_MACHINE:
      if ( extra_.event_.machine_pt_.machine != 0 ) {
        extra_.event_.machine_pt_.machine->trigger( extra_.event_.event );
      }
      return true;
  }
  return true;
}

/*
 * pull( v, up, def_value ) - Calls a machine's state method or a callback
 *
 */

int atm_connector::pull( UNUSED int v /* = 0 */, UNUSED int up /* = 0 */, bool def_value /* = false */ ) {
  switch ( mode_flags & 0x07 ) { //0b00000111
    case MODE_PULLCB:
      return ( *extra_.cb_callback_.cb_type_.pull_callback )( extra_.cb_callback_.callback_idx );
    case MODE_MACHINE:
      return extra_.event_.machine_pt_.machine->state();
  }
  return def_value;
}

/*
 * logOp() Returns the logical operator part of the mode_flags byte
 *
 */

int8_t atm_connector::logOp( void ) {
  return ( mode_flags & 0x18 ) >> 3; //0b00011000
}

/*
 * logOp() Returns the relational operator part of the mode_flags byte
 *
 */

int8_t atm_connector::relOp( void ) {
  return ( mode_flags & 0xE0 ) >> 5; //0b11100000
}

/*
 * set( callback, idx, logOp, relOp ) - Configures a connector object as a push callback
 *
 */

void atm_connector::set( atm_cb_push_t callback, int idx, int8_t logOp /* = 0 */, int8_t relOp /* = 0 */ ) {
  mode_flags = MODE_PUSHCB | ( logOp << 3 ) | ( relOp << 5 );
  extra_.cb_callback_.cb_type_.push_callback = callback;
  extra_.cb_callback_.callback_idx = idx;
}

/*
 * set( callback, idx, logOp, relOp ) - Configures a connector object as a pull callback
 *
 */

void atm_connector::set( atm_cb_pull_t callback, int idx, int8_t logOp /* = 0 */, int8_t relOp /* = 0 */ ) {
  mode_flags = MODE_PULLCB | ( logOp << 3 ) | ( relOp << 5 );
  extra_.cb_callback_.cb_type_.pull_callback = callback;
  extra_.cb_callback_.callback_idx = idx;
}

/*
 * set( callback, idx, logOp, relOp ) - Configures a connector object as a machine connector (calls trigger)
 *
 */

void atm_connector::set( Machine* m, int evt, int8_t logOp /* = 0 */, int8_t relOp /* = 0 */ ) {
  mode_flags = MODE_MACHINE | ( logOp << 3 ) | ( relOp << 5 );
  extra_.event_.machine_pt_.machine = m;
  extra_.event_.event = evt;
}

/*
 * mode() - Returns the mode part of the mode_flags byte
 *
 */

int8_t atm_connector::mode( void ) {
  return mode_flags & 0x07; //0b00000111
}
