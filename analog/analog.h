/*
 * analog.h
 *
 *  Created on: Mar. 30, 2019
 *      Author: benhillier
 */

#ifndef ANALOG_H_
#define ANALOG_H_

/*
 * pin number corresponds to the Arduino analog pins.
 * Works for pins 0-7 only.
 * Pin needs to be set as input (set corresponding DDR bit to 0).
 */
int analog_read(int pin);

#endif /* ANALOG_H_ */
