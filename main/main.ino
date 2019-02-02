#include <Servo.h>

#define FAST_TURN 10
#define SLOW_TURN 1

#define LOW_BOUND 200
#define DEAD_ZONE_LOW 400
#define DEAD_ZONE_HIGH 600
#define HIGH_BOUND 800

#define JS_L_X A4
#define JS_L_Y A5
#define JS_L_B  3

#define JS_R_X A6
#define JS_R_Y A7
#define JS_R_B  2

#define SERVO_PAN   7
#define SERVO_TILT  6 
#define MIN_SERVO_TIME 1000
#define MAX_SERVO_TIME 2000

#define alpha 0.25

struct return_tuple {
  int delta_x_L;
  int delta_y_L;
  int delta_x_R;
  int delta_y_R;
};

int data_x_L, data_y_L, data_x_R, data_y_R = 507;
int data_btn_R, data_btn_L = 1;

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

void increment_servo_value(int* write_time, int delta_value){

  *write_time += delta_value;

  if (*write_time < MIN_SERVO_TIME){
    *write_time = MIN_SERVO_TIME;
  } else if (*write_time > MAX_SERVO_TIME){
    *write_time = MAX_SERVO_TIME;
  }
  
}
