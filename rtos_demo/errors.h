/*
 * errors.h
 *
 *  Created on: Mar. 11, 2019
 *      Author: benhillier
 */

#ifndef ERRORS_H_
#define ERRORS_H_

#include "periodic.h"
#include <avr/io.h>

#define GLOBAL_LATE_BUFFER 2
#define LATE_ERROR 1
#define REPEATED_LATE_ERROR 2
#define DEADLINE_MISSED_ERROR 3

void errorInit(task_cb handler);
void checkTaskLate(task_t task);

#endif /* ERRORS_H_ */
