/*
 * event.c
 *
 *  Created on: Mar. 5, 2019
 *      Author: rtulip
 */

#include "event.h"
#include <stdlib.h>

int EVENT_COUNT = 0;
event_list events = {NULL, NULL, 0};

int addEvent(int priority, long runtime, task_cb callback, void* state){

	event_t* newEvent = (event_t*) malloc(sizeof(event_t));
	event_node* newNode = (event_node*) malloc(sizeof(event_node));
	EVENT_COUNT++;

	newEvent->EID = EVENT_COUNT;
	newEvent->priority = priority;
	newEvent->runtime = runtime;
	newEvent->callback = callback;
	newEvent->state = state;

	newNode->event = newEvent;
	newNode->next = NULL;

	if (events.size == 0){

		events.head = newNode;
		events.tail = newNode;

	} else {

		events.tail->next = newNode;
		events.tail = events.tail->next;

	}

	events.size++;
	return EVENT_COUNT;

}

int removeEvent(int EID){

	event_node* current = events.head;
	event_node* previous = events.head;

	if (current == NULL){

		return 0;

	} else if (current->event->EID == EID) {

		events.head = events.head->next;
		free(current->event);
		free(current);
		events.size--;
		return 1;

	} else {

		while (current != NULL && current->event->EID != EID){
			previous = current;
			current = current->next;
		}

		if (current == NULL){

			return 0;

		}

		previous->next = current->next;
		free(current->event);
		free(current);
		events.size--;

		return 1;
	}

}
