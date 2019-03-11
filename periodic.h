/*
 * periodic.h
 *
 *  Created on: Mar. 5, 2019
 *      Author: benhillier
 */
#include "timer.h"
#include <avr/io.h>

#ifndef PERIODIC_H_
#define PERIODIC_H_

#define RAISE(x) PORTH |= (1 << x)
#define LOWER(x) PORTH &= ~(1 << x)


#define MAXTASKS 8

typedef void (*task_cb)();

#define PERIODIC 1
#define EVENT 0

typedef struct
{
	long period;
	long remaining_time;
	int is_running;
	task_cb callback;
	void* state;
	int priority;
} task_t;

void periodicInit();

void addPeriodicTask(int delay, int period, task_cb task, void* state);
int addDelayedEvent(int delay, void* task, void* state);

unsigned int periodicDispatch();

#endif /* PERIODIC_H_ */
