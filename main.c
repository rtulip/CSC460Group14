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
	for (volatile int i = 0; i < 20000; i++);
	LOWER(PORTH5);
	eventScheduled = 0;
}

void Ping(void* stateP) {
	RAISE(PORTH3);
	int x;
	if (!eventScheduled && addDelayedEvent(50, Event,stateP)){
		eventScheduled = 1;
	}
	pingState* state = ((struct pingState*)stateP);
	for (x = 0; x < state->val; ++x) {}
	LOWER(PORTH3);

}

void Pong(void* stateP) {
	RAISE(PORTH4);
	pingState* state = ((struct pingState*)stateP);
	for (int x=0;x<state->val;x++) {}
	LOWER(PORTH4);

}

void handleError(int error) {
	RAISE(PORTH6);
	LOWER(PORTH6);
}

int main() {
	DDRH = (1 << DDH3) | (1 << DDH4) | (1 << DDH5) | (1 << DDH6);
	schedulerInit(handleError);

	pingState s = {5000};

	addPeriodicTask(0, 50, Ping, 5, &s);
	addPeriodicTask(10, 50, Pong, 5, &s);

	schedulerRun();
}

