#ifndef decode_joystick_h
#define decode_joystick_h

#define FAST_TURN 3
#define MEDIUM_TURN 2
#define SLOW_TURN 1
#define STATIONARY 0

#define FAST_TURN_DELTA 15
#define MEDIUM_TURN_DELTA 10
#define SLOW_TURN_DELTA 5


struct joystick_values {
  int x_delta;
  int y_delta;
  int button;
  int id;
};

struct joystick_values decode_joystick_message(unsigned char encoding);
int calculate_joystick_delta(int speed, int dir);
#endif
