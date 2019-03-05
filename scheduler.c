/*
 * scheduler.c
 *
 *  Created on: Mar. 5, 2019
 *      Author: benhillier
 */
#include "scheduler.h"
#include "timer.h"

#define disableInterrupts()         asm volatile ("cli"::)
#define enableInterrupts()          asm volatile ("sei"::)

volatile unsigned char* KernelSp;
volatile unsigned char* CurrentSp;


extern void ExitKernel();


void schedulerInit() {
	timerInit();
	periodicInit();
}


void schedulerRun() {
	for (;;) {

		disableInterrupts();
		unsigned long idleTime = periodicDispatch();
		// schedule event;
		createTimeout(idleTime);
		ExitKernel();

	}
}
