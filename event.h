/*
 * event.h
 *
 *  Created on: Mar. 5, 2019
 *      Author: rtulip
 */
#ifndef EVENT_H_
#define EVENT_H_

typedef void (*task_cb)();


typedef struct
{

	unsigned int EID;
	int priority;
	long runtime;
	task_cb callback;
	void* state;

} event_t;

void eventInit();

void scheduleEvent(task_cb task, void* state, int priority);

unsigned int eventDispatch(event_t** task, unsigned long idleTime);

typedef struct event_node event_node;

struct event_node{

	event_node* next;
	event_t* event;

};

typedef struct {

	event_node* head;
	event_node* tail;
	int size;

} event_list;

int addEvent(int priority, long runtime, task_cb callback, void* state);
int removeEvent(unsigned int EID, event_t** task);
int numEvents();
#endif /* EVENT_H_ */
