#include "pan_and_tilt.h"

void increment_servo_value(int* write_time, int delta_value){

  *write_time += delta_value;

  if (*write_time < MIN_SERVO_TIME){
    *write_time = MIN_SERVO_TIME;
  } else if (*write_time > MAX_SERVO_TIME){
    *write_time = MAX_SERVO_TIME;
  }
  
}
