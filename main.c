#define F_CPU 16000000UL

#include "Project2/scheduler.h"
#include "pan_and_tilt/pan_and_tilt.h"
#include "stdlib.h"
#include "Project2/periodic.h"
#include "uart/uart.h"
#include "roomba/roomba.h"
#include "Project2/event.h"
#include "Project2/timer.h"
#include <util/delay.h>
#include "uart/uart2.h"
#include "light_sensor/light_sensor.h"
#include "analog/analog.h"
#include "decode_joystick/decode_joystick.h"
#include "mode.h"

#define MAX_VELOCITY_DELTA_UP 120
#define MAX_VELOCITY_DELTA_DOWN 120
#define MAX_RADIUS_DELTA 50

char roomba_packet = 0;
char servo_packet = -1;
int drive_mode_switched = 0;
int kill_PID = 0;

typedef struct drive_roomba_state {
	int velocity;
	int radius;
} drive_roomba_state;

typedef struct laser_state {
	int time_remaining;
	int turn_on_time;
	int is_on;
} laser_state;

void handleError(int error) {
	RAISE(PORTH6);
	LOWER(PORTH6);
}

void gradually_come_to_stop(drive_roomba_state* state) {
	if (state->velocity > 0) {
		int new_velocity = state->velocity - MAX_VELOCITY_DELTA_DOWN;
		if (new_velocity > 0) {
			state->velocity = new_velocity;
		} else {
			state->velocity = 0;
		}
	} else {
		int new_velocity = state->velocity + MAX_VELOCITY_DELTA_DOWN;
		state->velocity = 0 < new_velocity ? 0 : new_velocity;
	}
	if (state->velocity == 0) {
		drive_mode_switched = 0;
	}

}

void updatePackets(void* none) {
	if (uart2_bytes_received() >= 2) {
		char p1 = uart2_get_byte(0);
		char p2 = uart2_get_byte(1);
		uart2_reset_receive();
		if (p1 & 0x80) {
			servo_packet = p1;
		} else {
			roomba_packet = p1;
		}
		if (p2 & 0x80) {
			servo_packet = p2;
		} else {
			roomba_packet = p2;
		}
	}
}

void updateServos(void* state) {
	joystick_values j = decode_joystick_message(servo_packet);
	servo_values values = get_servo_values(j);
	update_servo_pan(values.y_delta);
	update_servo_tilt(values.x_delta);
}

void updateLaser(laser_state* state) {
	joystick_values j = decode_joystick_message(servo_packet);
	int cur_time = millis();

	if (j.button) {
		// Turn off.
		PORTA &= ~(1 << PORTA4);
		if (state->is_on) {
			state->time_remaining -= cur_time - state->turn_on_time;
			state->is_on = 0;
		}
	} else if (state->is_on) {
		int time_remaining = state->time_remaining - (cur_time - state->turn_on_time);
		if (time_remaining <= 0) {
			// Turn off.
			PORTA &= ~(1 << PORTA4);
			state->time_remaining = 0;
			state->is_on = 0;
		}
		// if still time remaining, but is on then do nothing.
	} else if (state->time_remaining > 0){
		// Turn on.
		PORTA |= (1 << PORTA4);
		state->is_on = 1;
		state->turn_on_time = cur_time;
	}
}

void driveRoomba(drive_roomba_state* state) {
	joystick_values j = decode_joystick_message(roomba_packet);
	drive_values values = get_drive_values(j);
	if (drive_mode_switched) {
		gradually_come_to_stop(state);
	} else {
		// gradually increases speed.
		// gradual change from DRIVE_MODE to STATIONARY_MODE
		// does not work so only gradually change in DRIVE_MODE.
//		if (values.velocity > state->velocity + MAX_VELOCITY_DELTA_DOWN) {
//			state->velocity += MAX_VELOCITY_DELTA_DOWN;
//		} else if (values.velocity > state->velocity - MAX_VELOCITY_DELTA_UP){
//			state->velocity = values.velocity;
//		} else {
//			state->velocity -= MAX_VELOCITY_DELTA_UP;
//		}
		state->velocity = values.velocity;
		state->radius = values.radius;
	}
	Roomba_Drive(state->velocity, state->radius);
}

void switch_modes(void* none) {
	global_mode = 1 - global_mode;
	drive_mode_switched = 1;
}

void registerShot(void* None){
	RAISE(PORTH4);
	while(1);
	LOWER(PORTH4);
}

void lightSensor(void* none) {
	int v = getLightSensorValue();
	uart2_putchar(v);

	if (lightSensorIsLit()){

		RAISE(PORTH3);
		if (!kill_PID){
			kill_PID = addDelayedEvent(2000, 0, registerShot, NULL);
		}
		LOWER(PORTH3);

	} else {

		if (kill_PID){
			RAISE(PORTH5);
			removeDelayedEvent(kill_PID);
			kill_PID = 0;
			LOWER(PORTH5);
		}

	}
}


int main() {
	DDRH = (1 << DDH3) | (1 << DDH4) | (1 << DDH5) | (1 << DDH6);
	LOWER(PORTH3);
	LOWER(PORTH4);
	LOWER(PORTH5);
	LOWER(PORTH6);
	// Set pin 26 as output for laser.
	DDRA |= 1 << DDA4;
	disableInterrupts();
	Roomba_Init();
	_delay_ms(1000);
	enableInterrupts();
	pan_and_tilt_init();
	uart2_init(UART_9600);
	schedulerInit(handleError);
	lightSensorInit();
	addPeriodicTask(20, 100, lightSensor, 10, NULL);
	drive_roomba_state r_state = {0,0};

	laser_state l_state = {10000,0,0};
	addPeriodicTask(20, 100, updatePackets, 10, NULL);
	addPeriodicTask(40, 100, driveRoomba, 10, &r_state);
	addPeriodicTask(60, 100, updateServos, 10, NULL);
	addPeriodicTask(80, 100, updateLaser, 10, &l_state);
//	addPeriodicTask(30000, 30000, switch_modes, 10, NULL);
	schedulerRun();
	// Configure PORT D bit 0 to an output


//	lightSensorInit();

	return 0;
}
