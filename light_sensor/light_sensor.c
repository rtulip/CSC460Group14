#include "light_sensor.h"
#include <avr/io.h>
#include "../analog/analog.h"


int laserHitValue = 0;
int curLightValue;


void lightSensorInit() {
	// Set PORTC0 as input
	DDRC &= ~(1 << PORTC0);

	// Set initial curLightValue
	curLightValue = analog_read(0);
	for (int i = 0; i < 10; i++) {
		int currentLight = analog_read(0);
		laserHitValue = laserHitValue > currentLight ? laserHitValue : currentLight;
	}
	// Add 10% buffer to laserHitValue.
	laserHitValue += laserHitValue / 10;
}

int lightSensorIsLit(void* none) {
	curLightValue = (0.75 * (float) curLightValue) + (0.25 * (float) analog_read(0));
	return curLightValue > laserHitValue;
}
