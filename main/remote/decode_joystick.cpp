#include "decode_joystick.h"

struct joystick_values decode_joystick_message(unsigned char encoding) {
  int x_speed = (encoding >> 3) & 0x03;
  int x_dir = (encoding >> 5) & 0x01;
  int x = calculate_joystick_delta(x_speed, x_dir);
  int y_speed = encoding & 0x03;
  int y_dir = (encoding >> 2) & 0x01;
  int y = calculate_joystick_delta(y_speed, y_dir);
  int button = (encoding >> 6) & 0x01;
  int id = encoding >> 7;
  return joystick_values {x, y, button, id};
}

int calculate_joystick_delta(int speed, int dir) {
  int dir_modifier = dir == 0 ? -1 : 1;
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
