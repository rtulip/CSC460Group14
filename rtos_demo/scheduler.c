/*
 * scheduler.c
 *
 *  Created on: Mar. 5, 2019
 *      Author: benhillier
 */
#include "scheduler.h"
#include <avr/io.h>
#include "timer.h"
#include "errors.h"
#include <stdlib.h>

#define disableInterrupts()         asm volatile ("cli"::)
#define enableInterrupts()          asm volatile ("sei"::)

volatile unsigned char* KernelSp;
volatile unsigned char* CurrentSp;
event_t* CurrentEvent;


event_t* CurrentEvent;



extern void ExitKernel();


void schedulerInit(task_cb error_handler) {
	timerInit();
	periodicInit();
	eventInit();
	errorInit(error_handler);
}


void schedulerRun() {
	for (;;) {
		disableInterrupts();
		unsigned long idleTime = periodicDispatch();
		enableInterrupts();
		if (idleTime){

			if (eventDispatch(&CurrentEvent, idleTime)){

				CurrentEvent->callback(CurrentEvent->state);
				free(CurrentEvent);

			}


		}

	}
}
