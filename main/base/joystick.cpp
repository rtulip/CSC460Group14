#include "joystick.h"

int data_x_L, data_y_L, data_x_R, data_y_R = 500;
int data_btn_R, data_btn_L = 0;

void setup_joysticks(){
  pinMode (JS_L_X, INPUT);
  pinMode (JS_L_Y, INPUT);
  pinMode (JS_L_B, INPUT_PULLUP);
  pinMode (JS_R_X, INPUT);
  pinMode (JS_R_Y, INPUT);
  pinMode (JS_R_B, INPUT_PULLUP);
}

int calculate_delta(int data){

  if (data < LOW_BOUND) {
    return -FAST_TURN;
  } else if (data < DEAD_ZONE_LOW) {
    return -SLOW_TURN;
  } else if (data < DEAD_ZONE_HIGH) {
    return 0;
  } else if (data < HIGH_BOUND) {
    return SLOW_TURN;
  } else {
    return FAST_TURN;
  }
  
}

struct return_tuple get_increment_values(){
  // Read data from Joysticks
  data_x_L = alpha * data_x_L + (1-alpha) * analogRead (JS_L_X);
  data_y_L = alpha * data_y_L + (1-alpha) * analogRead (JS_L_Y);
  data_btn_L = digitalRead (JS_L_B);
  data_x_R = alpha * data_x_R + (1-alpha) * analogRead (JS_R_X);
  data_y_R = alpha * data_y_R + (1-alpha) * analogRead (JS_R_Y);
  data_btn_R = digitalRead (JS_R_B);

  int delta_x_L, delta_y_L, delta_x_R, delta_y_R;

  delta_x_L = calculate_delta(data_x_L);
  delta_y_L = calculate_delta(data_y_L);
  delta_x_R = calculate_delta(data_x_R);
  delta_y_R = calculate_delta(data_y_R);
  
  return return_tuple { delta_x_L, delta_y_L, delta_x_R, delta_y_R};

}

int get_button_data(int right){
  if (right){
    return digitalRead(JS_R_B);
  } else {
    return digitalRead(JS_L_B);
  }
}
