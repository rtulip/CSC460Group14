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

/*
 * Creates a timer that will interrupt after the specified
 * number of milliseconds. The interrupt will context switch
 * back into the Kernel.
 * WARNING: Does not work for idleTimes larger then 1048.
 */
void createTimeout(unsigned long idleTime) {
	TCCR4A = 0;
	//Set prescaller to 256
	TCCR4B = (1<<CS32);
	OCR4A = (idleTime / 2) * 125;
	TCNT4 = 0;
	TIMSK4 |= (1<<OCIE4A);
}

ISR (TIMER3_COMPA_vect)
{
	PORTH |= 1 << PORTH6;
	milliseconds += (TCNT3/ 125) * 2;
	TCNT3 %= 125;
	PORTH &= ~(1 << PORTH6);
}

// Timeout Interrupt
ISR (TIMER4_COMPA_vect)
{
	TIMSK4 = 0;
	TaskTimeout();
}
