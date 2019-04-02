/*
 * analog.c
 *
 *  Created on: Mar. 30, 2019
 *      Author: benhillier
 */
#include <avr/io.h>
#include "analog.h"
#include "../uart/uart2.h"
#include <util/delay.h>


/*
 * pin number corresponds to the Arduino analog pins.
 * Works for pins 0-7 only.
 * Pin needs to be set as input (set corresponding DDR bit to 0).
 */
int analog_read(int pin) {
	ADCSRA = 0b10000111;
	ADMUX = 0b01100000 | (0b00011111 & pin);
	ADCSRA |= 1 << ADSC;
	// Wait until the ADSC bit has been cleared
	while(ADCSRA & (1 << ADSC));
	return ADCH;
}
