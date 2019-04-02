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


#ifndef UART2_H_
#define UART2_H_




volatile uint8_t uart2_rx; 		// Flag to indicate uart received a byte

void uart2_init(UART_BPS bitrate);
void uart2_putchar(char c);
char uart2_get_byte(int index);
void uart2_putstr(char *s);

uint8_t uart2_bytes_received(void);
void uart2_reset_receive(void);


#endif /* UART2_H_ */
