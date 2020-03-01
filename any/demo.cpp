#include "demo.h"
// Точка входа в основную программу.
#ifdef USART_STREAM_ON
#include <USART0Minimal.h>
#endif

#include <avr/pgmspace.h>

#ifdef USART_STREAM_ON
static int uart_putchar(char character, FILE *stream) {
  if (character == '\n')
	transmitUSART0('\r');
  transmitUSART0(character);
  return 0;
}
#ifdef USART_STREAM_INPUT_ON
static int uart_getchar(FILE *stream) {
	return uart.read();
}
#endif
static FILE usart0_stream;
#endif

int main() {

#ifdef USART_STREAM_ON
  // There was a recent discussion on AVRfreaks,
  // the upshot of which was that the FDEV_SETUP_STREAM macro uses features
  // that don't work in C++ (but work fine in C.)
  //
  //http://www.avrfreaks.net/forum/stdio-setup-printf-and-pulling-my-hair-out
#ifndef USART_STREAM_INPUT_ON
  fdev_setup_stream(&usart0_stream, uart_putchar, nullptr, _FDEV_SETUP_WRITE);
#else
  fdev_setup_stream(&usart0_stream, uart_putchar, uart_getchar, _FDEV_SETUP_RW);
#endif
  //assign our stream to standart I/O streams
  stdin = stdout = &usart0_stream;
#endif

  initSystem();
  initSystemClock();

#ifdef USART_STREAM_ON
  initUSART0(9600);
#endif

  while (true) {
	//
  }
  return 0;
}
