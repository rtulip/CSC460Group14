/*
 * light_sensor.h
 *
 *  Created on: Mar. 28, 2019
 *      Author: benhillier
 */

#ifndef LIGHT_SENSOR_H_
#define LIGHT_SENSOR_H_

void lightSensorInit();
void updateLightSensorValue(void* none);
int lightSensorIsLit();

#endif /* LIGHT_SENSOR_H_ */
