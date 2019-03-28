#include <avr/io.h>
#include "uart.h"

/*
 Global Variables:
 Variables appearing in both ISR/Main are defined as 'volatile'.
*/
static volatile int rxn; // buffer 'element' counter.
static volatile char rx[UART_BUFFER_SIZE]; // buffer of 'char'.

void uart_putchar (char c)
{
	cli();
	while ( !( UCSR1A & (1<<UDRE1)) ); // Wait for empty transmit buffer
	UDR1 = c;  // Putting data into the buffer, forces transmission
	sei();
}

char uart_get_byte(int index)
{
	if (index < UART_BUFFER_SIZE) {
		return rx[index];
	}
	return 0;
}

void uart_putstr(char *s)
{
	while(*s) uart_putchar(*s++);

}

void uart_init(UART_BPS bitrate){

	DDRB = 0xff;
	PORTB = 0xff;

	rxn = 0;
	uart_rx = 0;

	/* Set baud rate */
	UBRR1H = 0;
	switch (bitrate) {
	case UART_19200:
		UBRR1L = 51;
		break;
	case UART_38400:
		UBRR1L = 25;
		break;
	case UART_57600:
		UBRR1L = 16;
		break;
	default:
		UBRR1L = 51;
	}
	/* Enable receiver and transmitter */
	UCSR1B = _BV(RXEN1)|_BV(TXEN1) | _BV(RXCIE1);

	/* Set frame format: 8data, 2stop bit */
	UCSR1C = (1<<UCSZ10) | _BV(UCSZ11);
}

uint8_t uart_bytes_received(void)
{
	return rxn;
}

void uart_reset_receive(void)
{
	rxn = 0;
}

/*
 Interrupt Service Routine (ISR):
*/

ISR(USART1_RX_vect)
{
	while ( !(UCSR1A & (1<<RXC1)) );

	//PORTB = ~_BV(PINB1);

	rx[rxn] = UDR1;
	rxn = (rxn + 1) % UART_BUFFER_SIZE;
	uart_rx = 1; // notify main of receipt of data.
	//PORTB = PORTB | _BV(PINB1);
}


