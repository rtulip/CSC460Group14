#include <avr/interrupt.h>
#include <stdio.h>
#include "timer.h"
#include "periodic.h"
#include "scheduler.h"
#include "stdlib.h"
/*
 * main.c
 *
 *  Created on: Mar. 4, 2019
 *      Author: benhillier
 */

/*
 * scheduler.cpp
 *
 *  Created on: 17-Feb-2011
 *      Author: nrqm
 *      Based on code by Jacob Schwartz
 */




typedef struct  {
	int val;
} pingState;

int eventScheduled = 0;

void Event(void* stateP){

	RAISE(PORTH5);
	for (volatile int i = 0; i < 7000; i++);
	LOWER(PORTH5);
	eventScheduled = 0;
}

void Ping(void* stateP) {
	RAISE(PORTH3);
	int x;
	if (!eventScheduled && addDelayedEvent(50, Event,stateP)){
		eventScheduled = 1;
		LOWER(PORTH3);
		RAISE(PORTH4);
		for (volatile int i; i < 50; i++);
		LOWER(PORTH4);
		RAISE(PORTH3);

		//addEvent(1, 9, Event, stateP);

	}
	pingState* state = ((struct pingState*)stateP);
	for (x = 0; x < state->val; ++x) {}

}

void Pong(void* stateP) {
	LOWER(PORTH3);
	pingState* state = ((struct pingState*)stateP);
	for (int x=0;x<state->val;x++) {}

}

int main() {
	DDRH = (1 << DDH3) | (1 << DDH4) | (1 << DDH5) | (1 << DDH6);
	schedulerInit();

	pingState s = {5000};

	addPeriodicTask(0, 50, Ping, &s);
	addPeriodicTask(10, 50, Pong, &s);

	schedulerRun();
}

