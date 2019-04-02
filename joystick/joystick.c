#include "joystick.h"
#include <avr/io.h>
#include "../analog/analog.h"
#include "../uart/uart2.h"

int data_x_L, data_y_L, data_x_R, data_y_R = 512;
int data_btn_R, data_btn_L = 0;

void setupJoysticks(){
	DDRC &= ~(1 << PORTC0) & ~(1 << PORTC1) & ~(1 << PORTC2) & ~(1 << PORTC3);
}

char getTurnRate(int data) {

  if (data < LOW_BOUND){
    return BACKWARD | FAST_TURN;
  } else if (data < MED_LOW_BOUND){
    return BACKWARD | MED_TURN;
  } else if (data < DEAD_ZONE_LOW){
    return BACKWARD | SLOW_TURN;
  } else if (data < DEAD_ZONE_HIGH){
    return FORWARD | NO_TURN;
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
  data_x_L = alpha * data_x_L + (1-alpha) * analog_read (JS_L_X);
  data_y_L = alpha * data_y_L + (1-alpha) * analog_read (JS_L_Y);
  data_x_R = alpha * data_x_R + (1-alpha) * analog_read (JS_R_X);
  data_y_R = alpha * data_y_R + (1-alpha) * analog_read (JS_R_Y);

  char turn_x_L, turn_y_L, turn_x_R, turn_y_R;

  turn_x_L = getTurnRate(data_x_L);
  turn_y_L = getTurnRate(data_y_L);
  turn_x_R = getTurnRate(data_x_R);
  turn_y_R = getTurnRate(data_y_R);

  returnTuple r = { turn_x_L, turn_y_L, turn_x_R, turn_y_R };
  return r;

}

void createPackets(unsigned char* leftPacket, unsigned char* rightPacket){
	returnTuple data = getTurnValues();
//  *leftPacket  = LEFT_JS_ID  | getButtonData(LEFT_JS_ID)  | (data.turn_x_L << 3) | data.turn_y_L;
//  *rightPacket = RIGHT_JS_ID | getButtonData(RIGHT_JS_ID) | (data.turn_x_R << 3) | data.turn_y_R;
  *leftPacket  = LEFT_JS_ID  | (data.turn_x_L << 3) | data.turn_y_L;
  *rightPacket = RIGHT_JS_ID | (data.turn_x_R << 3) | data.turn_y_R;
}

//char getButtonData(int right){
//  if (right){
//    return (char) digitalRead(JS_R_B) << 6;
//  } else {
//    return (char) digitalRead(JS_L_B) << 6;
//  }
//}
