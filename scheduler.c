/*
 * scheduler.c
 *
 *  Created on: Mar. 5, 2019
 *      Author: benhillier
 */
#include "scheduler.h"
#include <avr/io.h>
#include "timer.h"

#define disableInterrupts()         asm volatile ("cli"::)
#define enableInterrupts()          asm volatile ("sei"::)

volatile unsigned char* KernelSp;
volatile unsigned char* CurrentSp;
event_t* CurrentEvent;



extern void ExitKernel();


void schedulerInit() {
	timerInit();
	periodicInit();
	eventInit();
}


void schedulerRun() {
	for (;;) {

		disableInterrupts();
		LOWER(PORTH5);
		unsigned long idleTime = periodicDispatch();
		// schedule event;
		//createTimeout(idleTime);
		//ExitKernel();
		enableInterrupts();
		RAISE(PORTH5);
		if (idleTime){

			eventDispatch();

		}

	}
}
