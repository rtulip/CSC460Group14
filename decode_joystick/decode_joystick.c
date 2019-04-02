#include "decode_joystick.h"


struct joystick_values decode_joystick_message(unsigned char encoding) {
  int x_speed = (encoding >> 3) & 0x03;
  int x_dir = (encoding >> 5) & 0x01;
//  int x_delta = calculate_joystick_delta(x_speed, x_dir);
  int y_speed = encoding & 0x03;
  int y_dir = (encoding >> 2) & 0x01;
//  int y_delta = calculate_joystick_delta(y_speed, y_dir);
  int button = (encoding >> 6) & 0x01;
  int id = encoding >> 7;
  joystick_values values = {x_speed, x_dir, y_speed, y_dir, button, id};
  return values;
}

int get_velocity(int v) {
	if (v == FAST_TURN) {
		return FAST_VELOCITY;
	} else if (v == MEDIUM_TURN) {
		return MEDIUM_VELOCITY;
	} else if (v == SLOW_TURN) {
		return SLOW_VELOCITY;
	} else {
		return 0;
	}
}

int get_radius(int speed, int dir) {
	int dir_modifier = dir == BACKWARD ? -1 : 1;
	if (speed == FAST_TURN) {
		return FAST_RADIUS * dir_modifier;
	} else if (speed == MEDIUM_TURN) {
		return MEDIUM_RADIUS * dir_modifier;
	} else if (speed == SLOW_TURN) {
		return SLOW_RADIUS * dir_modifier;
	} else {
		// go straight.
		return 0x8000;
	}
}

drive_values get_drive_values(joystick_values values) {
	drive_values result;
	if (values.y_speed == STATIONARY && values.x_speed == STATIONARY) {
		result.velocity = 0;
		result.radius = 0;

	// Rotate if in stationary mode or the forward speed is 0.
	} else if (global_mode == STATIONARY_MODE || values.y_speed == STATIONARY) {
		result.radius = values.x_dir == BACKWARD ? -1 : 1;
		result.velocity = get_velocity(values.x_speed);

	} else {
		int dir_modifier = values.y_dir == BACKWARD ? 1 : -1;
		result.velocity = get_velocity(values.y_speed) * dir_modifier;
		result.radius = get_radius(values.x_speed, values.x_dir);
	}
	return result;
}

/**
 * For use with servo.
 */
int calculate_joystick_delta(int speed, int dir) {
  int dir_modifier = dir == BACKWARD ? -1 : 1;
  if (speed == FAST_TURN) {
    return FAST_TURN_DELTA * dir_modifier;
  } else if (speed == MEDIUM_TURN) {
    return MEDIUM_TURN_DELTA * dir_modifier;
  } else if (speed == SLOW_TURN) {
    return SLOW_TURN_DELTA * dir_modifier;
  } else {
    return 0;
  }
}

servo_values get_servo_values(joystick_values values) {
	int x_delta = calculate_joystick_delta(values.x_speed, values.x_dir);
	int y_delta = calculate_joystick_delta(values.y_speed, values.y_dir);
	servo_values result = {x_delta, y_delta, values.button, values.id};
	return result;
}
