/*
 * pan_and_tilt.h
 *
 *  Created on: Mar. 28, 2019
 *      Author: benhillier
 */

#ifndef PAN_AND_TILT_H_
#define PAN_AND_TILT_H_

void pan_and_tilt_init();
void update_servo_pan(int delta_value);
void update_servo_tilt(int delta_value);

#endif /* PAN_AND_TILT_H_ */
