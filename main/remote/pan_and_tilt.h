#ifndef pan_and_tilt_h
#define pan_and_tilt_h

void setup_pan_and_tilt();
void update_servo_pan(int delta_value);
void update_servo_tilt(int delta_value);
void increment_servo_value(int* write_time, int delta_value);

#endif
