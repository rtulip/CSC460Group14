#include "light_sensor.h"
#include <avr/io.h>
#include "../analog/analog.h"


int laserHitValue;
int curLightValue = 0;

void lightSensorInit() {
	// Set PORTC0 as input
	DDRF &= ~(1 << PORTF0);
	laserHitValue = 0;
	// Set initial curLightValue
	curLightValue = analog_read(0);
	for (int i = 0; i < 10; i++) {
		int currentLight = analog_read(0);
		laserHitValue = laserHitValue > currentLight ? laserHitValue : currentLight;
	}
	// Add 10% buffer to laserHitValue.
	laserHitValue += laserHitValue / 2;

}

int getLightSensorValue() {
	return curLightValue = (0.75 * (float) curLightValue) + (0.25 * (float) analog_read(0));
}

int getHitValue(){
	return laserHitValue;
}

int lightSensorIsLit() {
	return curLightValue > laserHitValue;
}
