#include "Arduino.h"
#define JS_L_X A4
#define JS_L_Y A5
#define JS_L_B  3

#define JS_R_X A6
#define JS_R_Y A7
#define JS_R_B  2

#define FAST_TURN 10
#define SLOW_TURN 1

#define alpha 0.25

#define LOW_BOUND 200
#define DEAD_ZONE_LOW 400
#define DEAD_ZONE_HIGH 600
#define HIGH_BOUND 800

struct return_tuple {
  int delta_x_L;
  int delta_y_L;
  int delta_x_R;
  int delta_y_R;
};

int calculate_delta(int data);
struct return_tuple get_increment_values();
