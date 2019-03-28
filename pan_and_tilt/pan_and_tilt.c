#include "pan_and_tilt.h"
#include <avr/io.h>

#define MIN_SERVO_TIME 1000
#define MAX_SERVO_TIME 2000


int servo_pan_time = 1500;
int servo_tilt_time = 1500;

// Pan is PWM pin 11. Tilt is PWM pin 12.
void pan_and_tilt_init() {
	// Set OCR1A and OCR1B output pins (PWM 11 and 12).
	DDRB = (1<<PB5) | (1<<PB6);

//	// set none-inverting mode, prescaler to 8, mode 8
	TCCR1A = (1<<COM1A1) | (1<<COM1B1);
	TCCR1B = (1<<CS11) | (1<<WGM13);

	// Set period to 20ms
	ICR1 = 20000;

	// Set duty cycles to 1.5ms
	OCR1A = 1500;
	OCR1B = 1500;
}

void update_servo_pan(int delta_value) {
	increment_servo_value(&servo_pan_time, delta_value);
	set_servo_pan(servo_pan_time);

}

void update_servo_tilt(int delta_value) {
  increment_servo_value(&servo_tilt_time, delta_value);
  set_servo_tilt(servo_tilt_time);
}

void increment_servo_value(int* write_time, int delta_value) {
  *write_time += delta_value;

  if (*write_time < MIN_SERVO_TIME) {
    *write_time = MIN_SERVO_TIME;
  } else if (*write_time > MAX_SERVO_TIME) {
    *write_time = MAX_SERVO_TIME;
  }
}

void set_servo_pan(int microseconds) {
	OCR1A = microseconds;
}

void set_servo_tilt(int microseconds) {
	OCR1B = microseconds;
}
