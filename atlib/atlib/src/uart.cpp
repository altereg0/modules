/**
 * Copyright (c) 2017, Lukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include "at/uart.hpp"

using namespace at;

UART::UART(uint16_t baudrate) : Stream() {
#if defined(__AVR_ATmega8__)

    uint16_t baudrate_calc = (F_CPU / 4 / baudrate - 1) / 2;

    /* Set PD1 (TXD) as output */
    DDRD |= _BV(PD1);

    /* Set calculated baud rate */
    UBRRH = baudrate_calc >> 8;
    UBRRL = baudrate_calc;

    /* Double the USART transmition speed */
    UCSRA = _BV(U2X);

    /* Enable transmition */
    UCSRB = _BV(TXEN);

    /* Set format 8N1 */
    UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);
#elif defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)


//    uint16_t baudrate_calc = (F_CPU / 4 / baudrate - 1) / 2;
    uint16_t baudrate_calc = ((F_CPU + baudrate * 8L) / (baudrate * 16L) - 1);


    /* Set PD1 (TXD) as output */
    DDRD |= _BV(PD1);

    /* Set calculated baud rate */
    UBRR0H = baudrate_calc >> 8;
    UBRR0L = baudrate_calc;

    /* Double the USART transmition speed */
    //UCSR0A = _BV(U2X0);

    /* Enable transmition */
    UCSR0B = _BV(TXEN0);

    //alter
    /* Enable transmition && receiption */
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);   // enable receiver and transmitter

    /* Set format 8N1 */
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);

#endif
}

void UART::write(uint8_t data) {
#if defined(__AVR_ATmega8__)
    /* Wait for empty transmit buffer */
    while (!(UCSRA & _BV(UDRE)));
    /* Put data into buffer, sends the data */
    UDR = data;
#elif defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
    /* Wait for empty transmit buffer */
//    while (!(UCSR0A & _BV(UDRE0)));
    loop_until_bit_is_set(UCSR0A, UDRE0);
    /* Put data into buffer, sends the data */
    UDR0 = data;
#endif
}

uint8_t UART::read(void) {
#if defined(__AVR_ATmega8__)
    return (0);
#else
//    return (0); //alter
    loop_until_bit_is_set(UCSR0A, RXC0);
//    while (!(UCSR0A & _BV(RXC0))); // wait while data is being received
    return UDR0; // return 8-bit data
#endif
}

