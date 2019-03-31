#ifndef decode_joystick_h
#define decode_joystick_h

#include "../mode.h"

#define FAST_TURN 3
#define MEDIUM_TURN 2
#define SLOW_TURN 1
#define STATIONARY 0

#define FORWARD 1
#define BACKWARD 0

#define FAST_TURN_DELTA 15
#define MEDIUM_TURN_DELTA 10
#define SLOW_TURN_DELTA 5

#define FAST_VELOCITY 500
#define MEDIUM_VELOCITY 200
#define SLOW_VELOCITY 50

#define FAST_RADIUS 50
#define MEDIUM_RADIUS 100
#define SLOW_RADIUS 300


typedef struct joystick_values {
  int x_speed;
  int x_dir;
  int y_speed;
  int y_dir;
  int button;
  int id;
} joystick_values;

typedef struct drive_values {
	int velocity;
	int radius;
} drive_values;

typedef struct stationary_values {

} stationary_values;

typedef struct servo_values {
  int x_delta;
  int y_delta;
  int button;
  int id;
} servo_values;

struct joystick_values decode_joystick_message(unsigned char encoding);
drive_values get_drive_values(joystick_values values);
servo_values get_servo_values(joystick_values values);

#endif
