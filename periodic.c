/*
 * periodic.c
 *
 *  Created on: Mar. 5, 2019
 *      Author: benhillier
 */
#include "periodic.h"


task_t tasks[MAXTASKS];

unsigned int last_runtime;
unsigned int num_tasks;
void * temp;
unsigned int min(unsigned int a, unsigned int b) {
	return a < b ? a : b;
}

void periodicInit() {
	last_runtime = millis();
	num_tasks = 0;
}

void addPeriodicTask(int delay, int period, task_cb task, void* state)
{
	static unsigned int id = 0;
	if (id < MAXTASKS)
	{
		tasks[id].remaining_time = delay;
		tasks[id].period = period;
		tasks[id].is_running = 1;
		tasks[id].callback = task;
		tasks[id].state = state;
		tasks[id].priority = PERIODIC;
		id++;
		num_tasks++;
	}
}

int addDelayedEvent(int delay,long runtime, void* task, void* state ){

	if (num_tasks < MAXTASKS){

		for (int i = 0; i < MAXTASKS; i++){
			if (!tasks[i].is_running){
				tasks[i].remaining_time = delay;
				tasks[i].period = 0;
				tasks[i].is_running = 1;
				tasks[i].callback = task;
				tasks[i].state = state;
				tasks[i].priority = EVENT;
				tasks[i].runtime = runtime;
				num_tasks++;
				return i+1;
				temp = task;
			}
		}
		return 0;
	} else {
		return 0;
	}

}

unsigned int periodicDispatch()
{
	unsigned int i;

	unsigned long now = millis();
	unsigned long elapsed = now - last_runtime;
	last_runtime = now;
	int taskToRun = -1;
	int eventToRun = -1;
	unsigned long idle_time = 0xFFFFFFFF;

	// update each task's remaining time, and identify the first ready task (if there is one).
	for (i = 0; i < MAXTASKS; i++)
	{

		if (tasks[i].is_running)
		{

			// update the task's remaining time
			tasks[i].remaining_time -= elapsed;
			if (tasks[i].remaining_time <= 0 && tasks[i].priority == PERIODIC)
			{
				if (taskToRun == -1)
				{
					// if this task is ready to run, and we haven't already selected a task to run,
					// select this one.
					taskToRun = i;
					tasks[i].remaining_time += tasks[i].period;
				}
				idle_time = 0;
			}
			else
			{
				idle_time = min((unsigned int)tasks[i].remaining_time, idle_time);
			}
		}
	}
	if (taskToRun == -1){
		for (i = 0; i < MAXTASKS; i++)
		{

			if (tasks[i].is_running)
			{

				// update the task's remaining time
				if (tasks[i].remaining_time <= 0 && tasks[i].priority == EVENT && tasks[i].runtime < idle_time) {
						// if this task is ready to run, and we haven't already selected a task to run,
						// select this one.
					taskToRun = i;
					idle_time = 0;
					break;
				} else {
					idle_time = min((unsigned int)tasks[i].remaining_time, idle_time);
				}
			}
		}
	}

	if (taskToRun != -1)
	{
		// If a task was selected to run, call its function.
		task_t t = tasks[taskToRun];
		t.callback(t.state);
		if (tasks[taskToRun].priority == EVENT){
			tasks[taskToRun].is_running = 0;
			num_tasks--;
		}
	}
	return idle_time;
}
