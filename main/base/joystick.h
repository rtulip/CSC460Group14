#include "Arduino.h"
#define JS_L_X A8
#define JS_L_Y A9
#define JS_L_B 52

#define JS_R_X A10
#define JS_R_Y A11
#define JS_R_B  53

#define FAST_TURN 0b11
#define MED_TURN  0b10
#define SLOW_TURN 0b01
#define NO_TURN   0b00
#define BACKWARD  0b000
#define FORWARD   0b100

#define alpha 0.25

#define LOW_BOUND      200
#define MED_LOW_BOUND  300
#define DEAD_ZONE_LOW  400
#define DEAD_ZONE_HIGH 600
#define MED_HIGH_BOUND 700
#define HIGH_BOUND     800

#define LEFT_JS_ID  0x00
#define RIGHT_JS_ID 0x80

struct returnTuple {
  char turn_x_L;
  char turn_y_L;
  char turn_x_R;
  char turn_y_R;
};

void setupJoysticks();
char getTurnRate(int data);
struct returnTuple getTurnValues();
char getButtonData(int right);
