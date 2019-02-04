#include "Arduino.h"
#define JS_L_X A8
#define JS_L_Y A9
#define JS_L_B  52

#define JS_R_X A10
#define JS_R_Y A11
#define JS_R_B  53

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

void setup_joysticks();
int calculate_delta(int data);
struct return_tuple get_increment_values();
int get_button_data(int right);
