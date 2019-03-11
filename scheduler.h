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


void schedulerInit();

void schedulerRun();

#endif /* SCHEDULER_H_ */
