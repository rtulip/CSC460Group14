#include <avr/interrupt.h>

/*
 * timer.c
 *
 *  Created on: Mar. 5, 2019
 *      Author: benhillier
 */

extern void TaskTimeout();

unsigned volatile long milliseconds;

unsigned long millis() {
	return milliseconds;
}

void timerInit() {
	TCCR3A = 0;
	//Set prescaller to 256
	TCCR3B = (1<<CS32);
	TCNT3 = 0;
	OCR3A = 125; // 2 ms
	TIMSK3 |= (1<<OCIE3A);
}

ISR (TIMER3_COMPA_vect)
{
	milliseconds += (TCNT3/ 125) * 2;
	TCNT3 %= 125;
}

