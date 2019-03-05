/*
 * timer.h
 *
 *  Created on: Mar. 5, 2019
 *      Author: benhillier
 */

#ifndef TIMER_H_
#define TIMER_H_

unsigned long millis();

void timerInit();

void createTimeout(unsigned long idleTime);

#endif /* TIMER_H_ */
