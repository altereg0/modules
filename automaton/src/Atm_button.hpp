#ifndef AUTOMATON_BUTTON_H_
#define AUTOMATON_BUTTON_H_

#include <Automaton.h>

template <board::DigitalPin attached_pin>
class Atm_button : public Machine {
 public:
  enum { IDLE, WAIT, PRESSED, REPEAT, RELEASE, LIDLE, LWAIT, LPRESSED, LRELEASE, WRELEASE, AUTO_ST };
  enum { EVT_LMODE, EVT_TIMER, EVT_DELAY, EVT_REPEAT, EVT_PRESS, EVT_RELEASE, EVT_COUNTER, EVT_AUTO, ELSE };
  enum { BTN_PASS4 = -4, BTN_PASS3 = -3, BTN_PASS2 = -2, BTN_PASS1 = -1, BTN_RELEASE = 0, BTN_PRESS1 = 1, BTN_PRESS2 = 2, BTN_PRESS3 = 3, BTN_PRESS4 = 4 };

  Atm_button( void ) : Machine(){};
  Atm_button& begin(){
      // clang-format off
      const static state_t state_table[] PROGMEM = {
          /* Standard Mode: press/repeat */
          /*                  ON_ENTER  ON_LOOP       ON_EXIT  EVT_LMODE  EVT_TIMER  EVT_DELAY  EVT_REPEAT EVT_PRESS  EVT_RELEASE  EVT_COUNTER   EVT_AUTO  ELSE */
          /* IDLE     */            -1,      -1,           -1,     LIDLE,        -1,        -1,         -1,     WAIT,          -1,          -1,   AUTO_ST,   -1,
          /* WAIT     */            -1,      -1,           -1,        -1,   PRESSED,        -1,         -1,       -1,        IDLE,          -1,        -1,    -1,
          /* PRESSED  */     ENT_PRESS,      -1,           -1,        -1,        -1,    REPEAT,         -1,       -1,     RELEASE,          -1,        -1,    -1,
          /* REPEAT   */     ENT_PRESS,      -1,           -1,        -1,        -1,        -1,     REPEAT,       -1,     RELEASE,          -1,        -1,    -1,
          /* RELEASE  */   ENT_RELEASE,      -1,           -1,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1,  IDLE,
          /* Long Press Mode: press/long press */
          /* LIDLE    */            -1,      -1,           -1,        -1,        -1,        -1,         -1,    LWAIT,          -1,          -1,        -1,    -1,
          /* LWAIT    */    ENT_LSTART,      -1,           -1,        -1,  LPRESSED,        -1,         -1,       -1,       LIDLE,          -1,        -1,    -1,
          /* LPRESSED */    ENT_LCOUNT,      -1,           -1,        -1,        -1,  LPRESSED,         -1,       -1,    LRELEASE,    WRELEASE,        -1,    -1,
          /* LRELEASE */  ENT_LRELEASE,      -1, EXT_WRELEASE,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1, LIDLE,
          /* WRELEASE */  ENT_LRELEASE,      -1, EXT_WRELEASE,        -1,        -1,        -1,         -1,       -1,       LIDLE,          -1,        -1,    -1,
          /* AUTO_ST   */      ENT_AUTO,      -1,           -1,        -1,        -1,        -1,         -1,       -1,          -1,          -1,        -1,  IDLE,
      };
      // clang-format on
      Machine::begin( state_table, ELSE );
      counter_longpress.set( 0 );
      timer_debounce.set( DEBOUNCE );
      timer_delay.set( ATM_TIMER_OFF );
      timer_repeat.set( ATM_TIMER_OFF );
      timer_auto.set( ATM_TIMER_OFF );
      return *this;
  };
  Atm_button& trace( Stream& stream ){
    setTrace( &stream, atm_serial_debug::trace,
              "BUTTON\0EVT_LMODE\0EVT_TIMER\0EVT_DELAY\0EVT_REPEAT\0EVT_PRESS\0EVT_RELEASE\0EVT_COUNTER\0EVT_"
              "AUTO_ST\0ELSE\0IDLE\0WAIT\0PRESSED\0REPEAT\0RELEASE\0LIDLE\0LWAIT\0LPRESSED\0LRELEASE\0WRELEASE\0AUTO" );
    return *this;
  };
  Atm_button& onPress( atm_cb_push_t callback, int idx = 0 ){
    onpress.set( callback, idx );
    return *this;
  };
  Atm_button& onPress( Machine& machine, int event = 0 ){
    onpress.set( &machine, event );
    return *this;
  };
  Atm_button& onPress( int id, atm_cb_push_t callback, int idx = 0 ){
    if ( id == 1 || id == 2 )
      longpress[id-1].set( callback, idx );
    return *this;
  };
  Atm_button& onPress( int id, Machine& machine, int event = 0 ){
    if ( id == 1 || id == 2 )
      longpress[id-1].set( &machine, event );
    return *this;
  };
  Atm_button& onRelease( atm_cb_push_t callback, int idx = 0 ){
    onrelease.set( callback, idx );
    return *this;
  };
  Atm_button& onRelease( Machine& machine, int event = 0 ){
    onrelease.set( &machine, event );
    return *this;
  };
  Atm_button& debounce(atm_timer_millis_t delay ){
    timer_debounce.set( delay );
    return *this;
  };
  Atm_button& longPress(int max, atm_timer_millis_t delay ){
    longpress_max = max;
    counter_longpress.set( longpress_max );
    timer_delay.set( delay );
    return *this;
  };
  Atm_button& repeat(atm_timer_millis_t delay = 500, atm_timer_millis_t speed = 50 ){
    timer_delay.set( delay );
    timer_repeat.set( speed );
    return *this;
  };
  Atm_button& autoPress(atm_timer_millis_t delay, int press = 1 ){
    auto_press = press;
    timer_auto.set( delay );
    return *this;
  };

 protected:
  enum { ENT_PRESS, ENT_RELEASE, ENT_LSTART, ENT_LCOUNT, ENT_LRELEASE, EXT_WRELEASE, ENT_AUTO };
  static const int DEBOUNCE = 5;
  atm_connector onpress, onrelease;
  atm_connector longpress[2];
  //  board::DigitalPin pin;
  gpio::FAST_PIN <attached_pin> pin{gpio::PinMode::INPUT_PULLUP};
  atm_timer_millis timer_debounce, timer_delay, timer_repeat, timer_auto;
  atm_counter counter_longpress;
  int longpress_max;
  int auto_press = 1;

  int event( int id ){
    switch ( id ) {
      case EVT_LMODE:
        return counter_longpress.value > 0;
      case EVT_TIMER:
        return timer_debounce.expired( this );
      case EVT_DELAY:
        return timer_delay.expired( this );
      case EVT_REPEAT:
        return timer_repeat.expired( this );
      case EVT_AUTO:
        return timer_auto.expired( this );
      case EVT_PRESS:
        return isPressed();
      case EVT_RELEASE:
        return isReleased();
      case EVT_COUNTER:
        return counter_longpress.expired();
    }
    return 0;
  };
  void action( int id ){
    int press;
    switch ( id ) {
      case ENT_PRESS:
        onpress.push( auto_press );
        longpress[0].push( 1 );
        return;
      case ENT_AUTO:
        onpress.push( 1 );
        longpress[0].push( 1 );
        return;
      case ENT_RELEASE:
      case EXT_WRELEASE:
        onrelease.push( 0 );
        return;
      case ENT_LSTART:
        counter_longpress.set( longpress_max );
        return;
      case ENT_LCOUNT:
        counter_longpress.decrement();
        press = ( longpress_max - counter_longpress.value );
        if ( onpress.mode() == atm_connector::MODE_PUSHCB ) {
          onpress.push( press * -1 );
        }
        return;
      case ENT_LRELEASE:
        press = ( longpress_max - counter_longpress.value );
        onpress.push( press );
        if ( press == 1 || press == 2 ) {
          longpress[press-1].push( press );
        }
        return;
    }
  };

  virtual void initButton(){};
  virtual bool isPressed(){ return !pin.value();};
  virtual bool isReleased(){ return pin.value();};
};


#endif