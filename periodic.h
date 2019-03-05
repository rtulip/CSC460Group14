/*
 * periodic.h
 *
 *  Created on: Mar. 5, 2019
 *      Author: benhillier
 */
#include "timer.h"

#ifndef PERIODIC_H_
#define PERIODIC_H_

#define MAXTASKS 8

typedef void (*task_cb)();


typedef struct
{
	long period;
	long remaining_time;
	int is_running;
	task_cb callback;
	void* state;
} task_t;

void periodicInit();

void addPeriodicTask(int delay, int period, task_cb task, void* state);

unsigned int periodicDispatch();

#endif /* PERIODIC_H_ */
