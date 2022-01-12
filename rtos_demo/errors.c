/*
 * errors.c
 *
 *  Created on: Mar. 11, 2019
 *      Author: benhillier
 */
#include "errors.h"
#include "event.h"

task_cb error_handler;

void errorInit(task_cb handler) {
	error_handler = handler;
}

void checkTaskLate(task_t task) {
	if (-task.remaining_time > GLOBAL_LATE_BUFFER) {
		++task.late_count;
		if (task.late_count >= 3) {
			error_handler(REPEATED_LATE_ERROR);
		} else {
			error_handler(LATE_ERROR);
		}
	} else if (-task.remaining_time > task.late_buffer) {
		error_handler(DEADLINE_MISSED_ERROR);
	} else {
		task.late_count = 0;
	}
}

