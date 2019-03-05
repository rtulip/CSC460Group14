#include <avr/interrupt.h>
#include <stdio.h>
#include "timer.h"
#include "periodic.h"
#include "scheduler.h"
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

void Ping(void* stateP) {
	PORTH |= 1 << PORTH4;
	int x;
	pingState* state = ((struct pingState*)stateP);
	for (x = 0; x < state->val; ++x) {}
	state->val = (state->val + 1000) % 10000;
	PORTH &= ~(1 << PORTH4);
}

void Pong(void* stateP) {
	PORTH |= 1 << PORTH3;
	int x;
	pingState* state = ((struct pingState*)stateP);
	for (x=0;x<state->val;x++) {}
	state->val = (state->val + 1000) % 10000;
	PORTH &= ~(1 << PORTH3);
}

int main() {
	DDRH = (1 << DDH3) | (1 << DDH4) | (1 << DDH5) | (1 << DDH6);
	schedulerInit();

	pingState s = {5000};

	addPeriodicTask(0, 50, Ping, &s);
	addPeriodicTask(10, 50, Pong, &s);

	schedulerRun();
}

