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

#define MAX_VELOCITY_DELTA_UP 80
#define MAX_VELOCITY_DELTA_DOWN 80
#define DEFAULT_PACKET 0b00100100

char roomba_packet = DEFAULT_PACKET;
char servo_packet = DEFAULT_PACKET;
int drive_mode_switched = 0;
int hit_wall;
int has_new_packets = 0;

roomba_sensor_data_t sensor_packet;

typedef struct drive_roomba_state {
	int velocity;
	int radius;
	int backup_count;
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
	} else {
		roomba_packet = DEFAULT_PACKET;
		servo_packet = DEFAULT_PACKET;
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

int sameRadius(int r1, int r2) {
	return r1 == r2 || (r1 != -1 && r1 != 1 && r2 != -1 && r2 != 1);
}

void driveRoomba(drive_roomba_state* state) {
	if (hit_wall) {
		state->backup_count = 5;
		hit_wall = 0;
	}
	if (state->backup_count) {
		// backup
		if (state->backup_count == 1) {
			Roomba_Drive(0,0);
		} else {
			Roomba_Drive(-100, 0x8000);
		}
		--state->backup_count;
		return;
	}
	joystick_values j = decode_joystick_message(roomba_packet);
	drive_values values = get_drive_values(j);
	if (drive_mode_switched || !sameRadius(state->radius, values.radius)) {
		gradually_come_to_stop(state);
		if (state->velocity == 0) {
			state->radius = values.radius;
		}
	} else {
		// gradually increases speed.
		// gradual change from DRIVE_MODE to STATIONARY_MODE
		// does not work so only gradually change in DRIVE_MODE.
		if (values.velocity > state->velocity + MAX_VELOCITY_DELTA_DOWN) {
			state->velocity += MAX_VELOCITY_DELTA_DOWN;
		} else if (values.velocity > state->velocity - MAX_VELOCITY_DELTA_UP){
			state->velocity = values.velocity;
		} else {
			state->velocity -= MAX_VELOCITY_DELTA_UP;
		}
		state->radius = values.radius;
	}
	Roomba_Drive(state->velocity, state->radius);


}

void switch_modes(void* none) {
	global_mode = 1 - global_mode;
	drive_mode_switched = 1;
}

void lightSensor(void* none) {
	int v = getLightSensorValue();
	uart2_putchar(v);

}

void updateRoombaSensor(void* none) {
	Roomba_UpdateSensorPacket(EXTERNAL, &sensor_packet);
}

void checkWalls(void* none) {
	if (sensor_packet.bumps_wheeldrops || sensor_packet.virtual_wall) {
		hit_wall = 1;
	}
}


int main() {
	DDRH = (1 << DDH3) | (1 << DDH4) | (1 << DDH5) | (1 << DDH6);
	// Set pin 26 as output for laser.
	DDRA |= 1 << DDA4;
	disableInterrupts();
	Roomba_Init();
	_delay_ms(1000);
	enableInterrupts();
	pan_and_tilt_init();
	uart2_init(UART_9600);
	schedulerInit(handleError);
//	addPeriodicTask(20, 100, lightSensor, 10, NULL);
	drive_roomba_state r_state = {0,0,0};

	laser_state l_state = {10000,0,0};
	addPeriodicTask(0, 100, updatePackets, 10, NULL);
	addPeriodicTask(10, 100, driveRoomba, 10, &r_state);
	addPeriodicTask(20, 100, updateServos, 10, NULL);
	addPeriodicTask(30, 100, updateLaser, 10, &l_state);
	addPeriodicTask(40, 100, updateRoombaSensor, 10, NULL);
	addPeriodicTask(70, 100, checkWalls, 10, NULL);
	addPeriodicTask(10000, 10000, switch_modes, 10, NULL);
	schedulerRun();
	// Configure PORT D bit 0 to an output


//	lightSensorInit();

	return 0;
}
