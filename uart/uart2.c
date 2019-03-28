#include <avr/io.h>
#include "uart.h"
#include "uart2.h"

/*
 Global Variables:
 Variables appearing in both ISR/Main are defined as 'volatile'.
*/
static volatile int rxn2; // buffer 'element' counter.
static volatile char rx2[UART_BUFFER_SIZE]; // buffer of 'char'.

void uart2_putchar (char c)
{
	cli();
	while ( !( UCSR2A & (1<<UDRE2)) ); // Wait for empty transmit buffer
	UDR2 = c;  // Putting data into the buffer, forces transmission
	sei();
}

char uart2_get_byte(int index)
{
	if (index < UART_BUFFER_SIZE) {
		return rx2[index];
	}
	return 0;
}

void uart2_putstr(char *s)
{
	while(*s) uart2_putchar(*s++);

}

void uart2_init(UART_BPS bitrate){

	DDRH = 0xff;
	PORTH = 0xff;

	rxn2 = 0;
	uart2_rx = 0;

	/* Set baud rate */
	UBRR2H = 0;
	switch (bitrate) {
	case UART_19200:
		UBRR2L = 51;
		break;
	case UART_38400:
		UBRR2L = 25;
		break;
	case UART_57600:
		UBRR2L = 16;
		break;
	default:
		UBRR2L = 51;
	}
	/* Enable receiver and transmitter */
	UCSR2B = _BV(RXEN2)|_BV(TXEN2) | _BV(RXCIE2);

	/* Set frame format: 8data, 2stop bit */
	UCSR2C = (1<<UCSZ20) | _BV(UCSZ21);
}

uint8_t uart2_bytes_received(void)
{
	return rxn2;
}

void uart2_reset_receive(void)
{
	rxn2 = 0;
}

/*
 Interrupt Service Routine (ISR):
*/

ISR(USART2_RX_vect)
{
	while ( !(UCSR2A & (1<<RXC2)) );

	//PORTB = ~_BV(PINB1);

	rx2[rxn2] = UDR2;
	rxn2 = (rxn2 + 1) % UART_BUFFER_SIZE;
	uart2_rx = 1; // notify main of receipt of data.
	//PORTB = PORTB | _BV(PINB1);
}


