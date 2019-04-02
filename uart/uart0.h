/*
 * uart0.h
 *
 *  Created on: Apr. 1, 2019
 *      Author: benhillier
 */

#ifndef UART0_H_
#define UART0_H_


/*
 * uart2.h
 *
 *  Created on: Mar. 28, 2019
 *      Author: benhillier
 */
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"



volatile uint8_t uart0_rx; 		// Flag to indicate uart received a byte

void uart0_init(UART_BPS bitrate);
void uart0_putchar(char c);
char uart0_get_byte(int index);
void uart0_putstr(char *s);

uint8_t uart0_bytes_received(void);
void uart0_reset_receive(void);


#endif /* UART0_H_ */
