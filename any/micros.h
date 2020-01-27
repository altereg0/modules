/*
* micros.h
*
* Created: 20.04.2018 7:25:41
*  Author: alter
*/


#ifndef MICROS_H_
#define MICROS_H_

//alter, bad patch
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

//from https://ucexperiment.wordpress.com/2012/03/16/examination-of-the-arduino-millis-function/

#if F_CPU > 16320000 // 16.32MHz - 65.28MHz
#define CLOCKSEL (_BV(CS20))
#define PRESCALER 256
#elif F_CPU > 2040000 // 2.04MHz - 16.32MHz
#define CLOCKSEL (_BV(CS01)|_BV(CS00))
#define PRESCALER 64
#elif F_CPU > 255 // 256Hz - 2.04MHz
#define CLOCKSEL (_BV(CS01))
#define PRESCALER 8
#endif

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000UL )
#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (F_CPU / 1000L) )
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(PRESCALER * 256))
// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

/**
* Milliseconds data type \n
* Data type				- Max time span			- Memory used \n
* unsigned char			- 255 milliseconds		- 1 byte \n
* unsigned int			- 65.54 seconds			- 2 bytes \n
* unsigned long			- 49.71 days			- 4 bytes \n
* unsigned long long	- 584.9 million years	- 8 bytes
*/
typedef unsigned long millis_t;
typedef unsigned long micros_t;

#define millis() millis_get()
#define micros() micros_get()

/**
* Initialise, must be called before anything else!
*/
void millis_init(void);

millis_t millis_get(void);

micros_t micros_get(void);



#endif /* MICROS_H_ */