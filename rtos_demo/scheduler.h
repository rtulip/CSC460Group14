/*
 * scheduler.h
 *
 *  Created on: Mar. 5, 2019
 *      Author: benhillier
 */

#include "periodic.h"
#include "event.h"
#include "timer.h"

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#define RAISE(x) PORTH |= (1 << x)
#define LOWER(x) PORTH &= ~(1 << x)
#define LATE_ERROR 1
#define REPEATED_LATE_ERROR 2
#define DEADLINE_MISSED_ERROR 3

void schedulerInit();

void schedulerRun();

#endif /* SCHEDULER_H_ */
