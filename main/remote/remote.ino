#include "scheduler.h"
#include "LightSensor.h"
#include <Servo.h>

#define SERVO_PAN   7
#define SERVO_TILT  6 
#define LASER_PIN   5
#define MIN_SERVO_TIME 1000
#define MAX_SERVO_TIME 2000

Servo servo_pan;
Servo servo_tilt;

int servo_pan_time = 1500;
int servo_tilt_time = 1500;
int pan_tilt_is_in_sync = 1;
void setup() {
  servo_pan.attach(SERVO_PAN, MIN_SERVO_TIME, MAX_SERVO_TIME);
  servo_tilt.attach(SERVO_TILT, MIN_SERVO_TIME, MAX_SERVO_TIME);

  Serial.begin(9600);
  Serial1.begin(9600);
  calibrateLightSensor();
  servo_pan.writeMicroseconds(servo_pan_time);
  Scheduler_Init();

  Scheduler_StartTask(0, 500, checkLightSensor);
  Scheduler_StartTask(0, 100, checkPanTiltValues);
}

void idle(uint32_t idle_period) {
  
}

void loop() {
  uint32_t idle_period = Scheduler_Dispatch();
  if (idle_period) {
    idle(idle_period);
  }
}

void checkLightSensor() {
  if (lightSensorIsLit()) {
    Serial1.write(1);  
  } else {
    Serial1.write(0);
  }  
}

void checkPanTiltValues() {
//  char b[4];
  
  if (Serial1.available() >= 14) {
    if (!pan_tilt_is_in_sync) {
      Serial.print("out of sync");
      char buf;
      while(buf != '\0') {
        buf = Serial1.read();
      }
      pan_tilt_is_in_sync = 1;
      Serial.print("in sync");
      return;
    }
    //String b = Serial1.readString();
    char message[14];
    Serial1.readBytes(message, 14);
    Serial.println(message);
    int pan, tilt, leftButton, rightButton;
    int numWritten = sscanf(message, "%d,%d,%d,%d\0", &pan, &tilt, &leftButton, &rightButton);
    if (numWritten != 4) {
      pan_tilt_is_in_sync = 0;
    }
    
    if (leftButton) {
      analogWrite(LASER_PIN, 255);
    } else {
      analogWrite(LASER_PIN, 0);
    }
    if (pan <= 2000 && pan >= 1000 && tilt <= 2000 && tilt >= 1000) {
       servo_pan.writeMicroseconds(pan);
       servo_tilt.writeMicroseconds(tilt);
    } else {
      Serial.print("no update ");
      Serial.print(pan);
      Serial.print(':');
      Serial.println(tilt);
    }
  }
}
