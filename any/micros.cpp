/*
* micros.cpp
*
* Created: 20.04.2018 7:25:58
*  Author: alter
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <util/atomic.h>
#include "micros.h"

#ifndef F_CPU
#error "F_CPU not defined!"
#endif

// Timer0

//#ifdef TIMER0_COMPA_vect
//#define ISR_VECT		TIMER0_COMPA_vect
//#endif

#define pwr_enable()	power_timer0_enable()
#define pwr_disable()	power_timer0_disable()

//static volatile millis_t milliseconds;

static volatile uint8_t timer0_fract;
static volatile millis_t timer0_millis;
static volatile millis_t timer0_overflow_count;

// Initialise library
void millis_init()
{
	// Timer settings
	TCCR0A = 0x00;
	TCCR0B = CLOCKSEL;
	TIMSK0 = (1 << TOIE0);
}

// Get current milliseconds
millis_t millis_get()
{
	millis_t ms;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		ms = timer0_millis;
	}
	return ms;
}


// Get current microseconds
micros_t micros_get()
{
	micros_t mc;
	uint8_t t;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		mc = timer0_overflow_count;
		t = TCNT0;
		if ((TIFR0 & _BV(TOV0)) && (t < 255))
		mc++;
	}
	return ((mc << 8) + t) * (64 / clockCyclesPerMicrosecond());
}

ISR(TIMER0_OVF_vect)
{
	// copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	millis_t m = timer0_millis;
	uint8_t f = timer0_fract;

	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m += 1;
	}
	timer0_fract = f;
	timer0_millis = m;
	timer0_overflow_count++;
}
