#include <Servo.h>
#include "joystick.h"
#include "scheduler.h"

#include <LiquidCrystal.h>
#define LCD_ROW_LEN 16
#define LCD_NUM_ROWS 2
int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
unsigned char leftJoystickPacket, rightJoystickPacket;

void setup (){

  /* 
   *  Setup LCD 
   */
  lcd.begin(LCD_ROW_LEN,LCD_NUM_ROWS);
  lcd.setCursor(0,0);
  lcd.print("Left  :");
  lcd.setCursor(0,1);
  lcd.print("Right :");

  /*
   *  Setup joysticks
   */
  setupJoysticks();
  
  /*
   *  Start serial communication
   */
  Serial.begin (9600); // 9600 bps
  Serial1.begin(9600);
  pinMode (48, OUTPUT);
  pinMode (49, OUTPUT);
  pinMode (50, OUTPUT);
  pinMode (51, OUTPUT);
  /*
   * Start scheduler
   */
  Scheduler_Init();
  Scheduler_StartTask(0,100,updateServoControl);
  Scheduler_StartTask(0,50,updateDisplay);
  Scheduler_StartTask(10,100,updateHit);

}

void idle(uint32_t idle_period) {
  digitalWrite(51,HIGH);
  delay(idle_period);
  digitalWrite(51,LOW);
}

void loop (){
  uint32_t idle_period = Scheduler_Dispatch();
  if (idle_period) {
    idle(idle_period);
  }
}

void updateServoControl(){
  digitalWrite(48,HIGH);
  returnTuple turnValues = getTurnValues();
  createPackets(turnValues, &leftJoystickPacket, &rightJoystickPacket);
  Serial1.write(leftJoystickPacket);
  Serial1.write(rightJoystickPacket);
  digitalWrite(48,LOW);
}

void createPackets(returnTuple data, unsigned char* leftPacket, unsigned char* rightPacket){

  *leftPacket  = LEFT_JS_ID  | getButtonData(LEFT_JS_ID)  | (data.turn_x_L << 3) | data.turn_y_L;
  *rightPacket = RIGHT_JS_ID | getButtonData(RIGHT_JS_ID) | (data.turn_x_R << 3) | data.turn_y_R;
}

void updateDisplay(){
  digitalWrite(49,HIGH);
  lcd.setCursor(7,0);
  lcd.print(leftJoystickPacket, OCT);
  lcd.setCursor(7,1);
  lcd.print(rightJoystickPacket, OCT);
  digitalWrite(49,LOW);
}

void updateHit(){
  digitalWrite(50,HIGH);
  if (Serial1.available()){
    lcd.setCursor(15,0);
    lcd.print(Serial1.read());
  }
  digitalWrite(50,LOW);
}
