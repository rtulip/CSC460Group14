#include "Arduino.h"
#include "LightSensor.h"

#define lightSensorPin A7

int laserHitValue = 0;
int curLightValue;

void calibrateLightSensor() {
	// Set initial curLightValue
	curLightValue = analogRead(lightSensorPin);
  for (int i = 0; i < 10; i++) {
    int currentLight = analogRead(lightSensorPin);
    laserHitValue = laserHitValue > currentLight ? laserHitValue : currentLight;
    delay(10);
  }
  // Add 10% buffer to laserHitValue.
  laserHitValue += laserHitValue / 10;
  Serial.print("hit val: ");
  Serial.println(laserHitValue);
}

int lightSensorIsLit() {
  curLightValue = (0.75 * (float) curLightValue) + (0.25 * (float) analogRead(lightSensorPin));
  Serial.println(curLightValue);
  return curLightValue > laserHitValue;
}
