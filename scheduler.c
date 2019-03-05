/*
 * scheduler.c
 *
 *  Created on: Mar. 5, 2019
 *      Author: benhillier
 */
#include "scheduler.h"

#define disableInterrupts()         asm volatile ("cli"::)
#define enableInterrupts()          asm volatile ("sei"::)


void schedulerInit() {
	timerInit();
	periodicInit();
}

void schedulerRun() {
	for (;;) {
		disableInterrupts();
		unsigned long idleTime = periodicDispatch();

		enableInterrupts();

	}
}
