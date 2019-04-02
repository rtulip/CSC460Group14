#include <avr/io.h>
#include "uart.h"
#include "uart0.h"

/*
 Global Variables:
 Variables appearing in both ISR/Main are defined as 'volatile'.
*/
static volatile int rxn0; // buffer 'element' counter.
static volatile char rx0[UART_BUFFER_SIZE]; // buffer of 'char'.

void uart0_putchar (char c)
{
	cli();
	while ( !( UCSR0A & (1<<UDRE0)) ); // Wait for empty transmit buffer
	UDR0 = c;  // Putting data into the buffer, forces transmission
	sei();
}

char uart0_get_byte(int index)
{
	if (index < UART_BUFFER_SIZE) {
		return rx0[index];
	}
	return 0;
}

void uart0_putstr(char *s)
{
	while(*s) uart0_putchar(*s++);

}

void uart0_init(UART_BPS bitrate){

	DDRH = 0xff;
	PORTH = 0xff;

	rxn0 = 0;
	uart0_rx = 0;

	/* Set baud rate */
	UBRR0H = 0;
	switch (bitrate) {
	case UART_19200:
		UBRR0L = 51;
		break;
	case UART_38400:
		UBRR0L = 25;
		break;
	case UART_57600:
		UBRR0L = 16;
		break;
	default:
		UBRR0L = 51;
	}
	/* Enable receiver and transmitter */
	UCSR0B = _BV(RXEN0)|_BV(TXEN0) | _BV(RXCIE0);

	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<UCSZ00) | _BV(UCSZ01);
}

uint8_t uart0_bytes_received(void)
{
	return rxn0;
}

void uart0_reset_receive(void)
{
	rxn0 = 0;
}

/*
 Interrupt Service Routine (ISR):
*/

ISR(USART0_RX_vect)
{
	while ( !(UCSR0A & (1<<RXC0)) );

	//PORTB = ~_BV(PINB1);

	rx0[rxn0] = UDR0;
	rxn0 = (rxn0 + 1) % UART_BUFFER_SIZE;
	uart0_rx = 1; // notify main of receipt of data.
	//PORTB = PORTB | _BV(PINB1);
}


