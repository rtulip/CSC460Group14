#include <Servo.h>
#include "joystick.h"
#include "pan_and_tilt.h"

Servo servo_pan;
Servo servo_tilt;

int servo_pan_time = 1500;
int servo_tilt_time = 1500;

void setup (){
  pinMode (JS_L_X, INPUT);
  pinMode (JS_L_Y, INPUT);
  pinMode (JS_L_B, INPUT_PULLUP);
  pinMode (JS_R_X, INPUT);
  pinMode (JS_R_Y, INPUT);
  pinMode (JS_R_B, INPUT_PULLUP);

  servo_pan.attach(SERVO_PAN, MIN_SERVO_TIME, MAX_SERVO_TIME);
  servo_tilt.attach(SERVO_TILT, MIN_SERVO_TIME, MAX_SERVO_TIME);
  
  Serial.begin (9600); // 9600 bps
  
}

void loop (){
  
  return_tuple increment_values = get_increment_values(); 
  increment_servo_value(&servo_pan_time, increment_values.delta_y_L);
  increment_servo_value(&servo_tilt_time, increment_values.delta_x_L);

  Serial.print(" Pan: ");
  Serial.print(servo_pan_time);
  Serial.print(", Tilt: ");
  Serial.println(servo_tilt_time);
  
  servo_pan.writeMicroseconds(servo_pan_time);
  servo_tilt.writeMicroseconds(servo_tilt_time);

  delay(30);
}
