#include "demo.h"
// Точка входа в основную программу.

/*
static int uart_putchar(char character, FILE *stream) {
    uart.write(character);
    return 0;
}
static int uart_getchar(FILE *stream) {
    return uart.read();
}

static FILE usart0_stream;
*/

#define N 8
uint32_t ema(uint32_t new_sample)
{
    static uint32_t samples[N];
    static uint32_t total;
    static uint8_t i = 0;

    total -= samples[i];
    total += new_sample;
    samples[i] = new_sample;
    i = (i+1) % N;
    uint32_t average = total >> 3;
    return average;
}

int main() {

	// There was a recent discussion on AVRfreaks,
	// the upshot of which was that the FDEV_SETUP_STREAM macro uses features
	// that don't work in C++ (but work fine in C.)
	//
	//http://www.avrfreaks.net/forum/stdio-setup-printf-and-pulling-my-hair-out
    fdev_setup_stream(&usart0_stream, uart_putchar, uart_getchar, _FDEV_SETUP_RW);
	//assign our stream to standart I/O streams
    stdin=stdout=&usart0_stream;


	while(true) {
		//
	}
    return 0;
}
