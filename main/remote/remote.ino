#include "scheduler.h"
#include "LightSensor.h"
#include "decode_joystick.h"
#include "pan_and_tilt.h"

#define LASER_PIN   5

void setup() {
  setup_pan_and_tilt();
  Serial.begin(9600);
  Serial1.begin(9600);
  calibrateLightSensor();

  Scheduler_Init();

  Scheduler_StartTask(0, 500, checkLightSensor);
  Scheduler_StartTask(0, 100, checkPanTiltValues);
}

void idle(uint32_t idle_period) {
  delay(idle_period);
}

void loop() {
  uint32_t idle_period = Scheduler_Dispatch();
  if (idle_period) {
    idle(idle_period);
  }
}

void checkLightSensor() {
  if (lightSensorIsLit()) {
    Serial1.write(1);
  } else {
    Serial1.write(0);
  }
}

void checkPanTiltValues() {
  unsigned char encoded_message = Serial1.read();
  struct joystick_values message = decode_joystick_message(encoded_message);
  if (message.button) {
    analogWrite(LASER_PIN, 0);
  } else {
    analogWrite(LASER_PIN, 255);
  }
  update_servo_pan(message.x_delta);
  update_servo_tilt(message.x_delta);
}
