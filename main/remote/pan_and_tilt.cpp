#include "pan_and_tilt.h"
#include <Servo.h>

#define MIN_SERVO_TIME 1000
#define MAX_SERVO_TIME 2000
#define SERVO_PAN   7
#define SERVO_TILT  6

Servo servo_pan;
Servo servo_tilt;

int servo_pan_time = 1500;
int servo_tilt_time = 1500;

void setup_pan_and_tilt() {
  servo_pan.attach(SERVO_PAN, MIN_SERVO_TIME, MAX_SERVO_TIME);
  servo_tilt.attach(SERVO_TILT, MIN_SERVO_TIME, MAX_SERVO_TIME);
  servo_pan.writeMicroseconds(servo_pan_time);
  servo_tilt.writeMicroseconds(servo_tilt_time);
}

void update_servo_pan(int delta_value) {
  increment_servo_value(&servo_pan_time, delta_value);
  servo_pan.writeMicroseconds(servo_pan_time);
}

void update_servo_tilt(int delta_value) {
  increment_servo_value(&servo_tilt_time, delta_value);
  servo_tilt.writeMicroseconds(servo_tilt_time);
}

void increment_servo_value(int* write_time, int delta_value) {
  *write_time += delta_value;

  if (*write_time < MIN_SERVO_TIME) {
    *write_time = MIN_SERVO_TIME;
  } else if (*write_time > MAX_SERVO_TIME) {
    *write_time = MAX_SERVO_TIME;
  }
}
