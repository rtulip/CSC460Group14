#include <scheduler.h>

#include <LightSensor.h>
#include <LiquidCrystal.h>

int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  calibrateLightSensor();

  Scheduler_Init();

  Scheduler_StartTask(0, 500, checkLightSensor);
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
    lcd.setCursor(0,0);
    lcd.write("We're Hit!!");  
  } else {
    lcd.setCursor(0,0);
    lcd.write("We're fine :)");
  }  
}
