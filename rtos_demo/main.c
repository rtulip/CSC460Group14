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

/* */
typedef struct  {
	int val;
} pingState;

int DELAYED = 1;

void Event(void* stateP){

	RAISE(PORTH5);
	for (volatile int i = 0; i < 20000; i++);
	//for (volatile int i = 0; i < 20000; i++);
	LOWER(PORTH5);
	DELAYED = 1;

}

void DelayedEvent(void* stateP){

	RAISE(PORTH6);
	for (volatile int i = 0; i < 20000; i++);
	for (volatile int i = 0; i < 20000; i++);
	LOWER(PORTH6);
	DELAYED  = 0;

}

void Ping(void* stateP) {
	RAISE(PORTH3);
	volatile int x;

	if (DELAYED){
		LOWER(PORTH3);
		int EID = addDelayedEvent(100, 50, DelayedEvent,stateP);
		RAISE(PORTH3);

	} else {
		int EID = addEvent(1, 25, Event, stateP);
	}

	pingState* state = ((struct pingState*)stateP);
	for (x = 0; x < state->val; ++x) {}
	LOWER(PORTH3);

}

void Pong(void* stateP) {
	RAISE(PORTH4);
	pingState* state = ((struct pingState*)stateP);
	for (volatile int x=0;x<state->val;x++) {}
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

