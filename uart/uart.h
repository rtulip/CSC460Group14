///**
// * @file   uart.c
// * @author Justin Tanner
// * @date   Sat Nov 22 21:32:03 2008
// *
// * @brief  UART Driver targetted for the AT90USB1287
// *
// */
//
//#ifndef __UART_H__
//#define __UART_H__
//
//#include <avr/interrupt.h>
//#include <stdint.h>

//
//#define UART_BUFFER_SIZE    32
//
//void uart_init(UART_BPS bitrate);
//void uart_putchar(uint8_t byte);
//uint8_t uart_get_byte(int index);
//uint8_t uart_bytes_received(void);
//void uart_reset_receive(void);
//
//#endif

#ifndef __UART_H__
#define __UART_H__
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>


typedef enum _uart_bps
{
	UART_9600,
	UART_19200,
	UART_38400,
	UART_57600,
	UART_DEFAULT,
} UART_BPS;

#define UART_BUFFER_SIZE 100			// size of Rx ring buffer.

volatile uint8_t uart_rx; 		// Flag to indicate uart received a byte

void uart_init(UART_BPS bitrate);
void uart_putchar(char c);
char uart_get_byte(int index);
void uart_putstr(char *s);

uint8_t uart_bytes_received(void);
void uart_reset_receive(void);
#endif

