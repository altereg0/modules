#ifndef AUTOMATON_DIGITAL_H_
#define AUTOMATON_DIGITAL_H_

#include <Automaton.h>
// Digital pin with a minimum duration in ms
// On detection another machine is messaged or a callback is fired

class Atm_digital : public Machine {
 public:
  enum { IDLE, WAITH, VHIGH, WAITL, VLOW };     // STATES
  enum { EVT_TIMER, EVT_HIGH, EVT_LOW, ELSE };  // EVENTS

  Atm_digital( void ) : Machine(){};
  Atm_digital& begin(board::DigitalPin pin_, atm_timer_millis_t debounce = 20, bool activeLow_ = false, bool pullUp_ = false );
  int state( void );
  Atm_digital& onChange( bool status, atm_cb_push_t callback, int idx = 0 );
  Atm_digital& onChange( bool status, Machine& machine, int event = 0 );
  Atm_digital& onChange( atm_cb_push_t callback, int idx = 0 );
  Atm_digital& onChange( Machine& machine, int event = 0 );
  Atm_digital& led( board::DigitalPin led, bool activeLow = false );
  Atm_digital& trace( Stream& stream );

 private:
  enum { ENT_HIGH, ENT_LOW };                             // ACTIONS
  enum { ON_CHANGE_FALSE, ON_CHANGE_TRUE, _CONN_SIZE_ };  // CONNECTORS
  board::DigitalPin pin;
  atm_timer_millis timer;
  bool activeLow;
  atm_connector connection[_CONN_SIZE_];
  board::DigitalPin indicator;
  bool indicatorActiveLow;

  int event( int id );
  void action( int id );
};

#endif