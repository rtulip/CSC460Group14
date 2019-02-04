#include <Servo.h>
#include "joystick.h"
#include "pan_and_tilt.h"
#include "scheduler.h"

#include <LiquidCrystal.h>
#define LCD_ROW_LEN 16
#define LCD_NUM_ROWS 2
int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Servo servo_pan;
Servo servo_tilt;

int servo_pan_time = 1500;
int servo_tilt_time = 1500;

void setup (){

  lcd.begin(LCD_ROW_LEN,LCD_NUM_ROWS);

  lcd.setCursor(0,0);
  lcd.print("Pan : ");
  lcd.setCursor(0,1);
  lcd.print("Tilt: ");
  
  setup_joysticks();
  servo_pan.attach(SERVO_PAN, MIN_SERVO_TIME, MAX_SERVO_TIME);
  servo_tilt.attach(SERVO_TILT, MIN_SERVO_TIME, MAX_SERVO_TIME);

  Serial.begin (9600); // 9600 bps
  Serial1.begin(9600);
  Scheduler_Init();
  Scheduler_StartTask(0,100,updateServoControl);
  Scheduler_StartTask(30,300,updateDisplay);
  Scheduler_StartTask(10,500,updateHit);
  
}

void idle(uint32_t idle_period) {
  delay(idle_period);
}

void loop (){
  uint32_t idle_period = Scheduler_Dispatch();
  if (idle_period) {
    idle(idle_period);
  }
}

void updateServoControl(){
  return_tuple increment_values = get_increment_values(); 
  increment_servo_value(&servo_pan_time, increment_values.delta_y_L);
  increment_servo_value(&servo_tilt_time, increment_values.delta_x_L);
  char msg[14];
  msg[0] = nthdigit(servo_pan_time,3);
  msg[1] = nthdigit(servo_pan_time,2);
  msg[2] = nthdigit(servo_pan_time,1);
  msg[3] = nthdigit(servo_pan_time,0);
  msg[4] = ',';
  msg[5] = nthdigit(servo_tilt_time,3);
  msg[6] = nthdigit(servo_tilt_time,2);
  msg[7] = nthdigit(servo_tilt_time,1);
  msg[8] = nthdigit(servo_tilt_time,0);
  msg[9] = ',';
  msg[10] = (char) get_button_data(0) + '0';
  msg[11] = ',';
  msg[12] = (char) get_button_data(1) + '0';
  msg[13] = '\0';
  
  Serial1.write(msg,14);
}

void updateDisplay(){
  lcd.setCursor(6,0);
  lcd.print(servo_pan_time, DEC);
  lcd.setCursor(6,1);
  lcd.print(servo_tilt_time, DEC);
}

void updateHit(){
  if (Serial1.available()){
    lcd.setCursor(15,0);
    lcd.print(Serial1.read());
  }
}

char nthdigit(int x, int n){
    static int powersof10[4] = {1, 10, 100, 1000};
    return ((x / powersof10[n]) % 10) + '0';
}
