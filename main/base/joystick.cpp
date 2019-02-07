#include "joystick.h"

int data_x_L, data_y_L, data_x_R, data_y_R = 512;
int data_btn_R, data_btn_L = 0;

void setupJoysticks(){
  pinMode (JS_L_X, INPUT);
  pinMode (JS_L_Y, INPUT);
  pinMode (JS_L_B, INPUT_PULLUP);
  pinMode (JS_R_X, INPUT);
  pinMode (JS_R_Y, INPUT);
  pinMode (JS_R_B, INPUT_PULLUP);
}

char getTurnRate(int data) {

  if (data < LOW_BOUND){  
    return BACKWARD | FAST_TURN;    
  } else if (data < MED_LOW_BOUND){
    return BACKWARD | MED_TURN;
  } else if (data < DEAD_ZONE_LOW){
    return BACKWARD | SLOW_TURN;
  } else if (data < DEAD_ZONE_HIGH){
    return NO_TURN;
  } else if (data < MED_HIGH_BOUND){
    return FORWARD | SLOW_TURN;
  } else if (data < HIGH_BOUND){
    return FORWARD | MED_TURN;
  } else {
    return FORWARD | FAST_TURN;
  }
}


struct returnTuple getTurnValues(){
  // Read data from Joysticks
  data_x_L = alpha * data_x_L + (1-alpha) * analogRead (JS_L_X);
  data_y_L = alpha * data_y_L + (1-alpha) * analogRead (JS_L_Y);
  data_x_R = alpha * data_x_R + (1-alpha) * analogRead (JS_R_X);
  data_y_R = alpha * data_y_R + (1-alpha) * analogRead (JS_R_Y);
  
  char turn_x_L, turn_y_L, turn_x_R, turn_y_R;

  turn_x_L = getTurnRate(data_x_L);
  turn_y_L = getTurnRate(data_y_L);
  turn_x_R = getTurnRate(data_x_R);
  turn_y_R = getTurnRate(data_y_R);
  
  return returnTuple { turn_x_L, turn_y_L, turn_x_R, turn_y_R };

}

char getButtonData(int right){
  if (right){
    return (char) digitalRead(JS_R_B) << 6;
  } else {
    return (char) digitalRead(JS_L_B) << 6;
  }
}
