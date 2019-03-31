#define F_CPU 16000000UL

#include "Project2/scheduler.h"
#include "pan_and_tilt/pan_and_tilt.h"
#include "stdlib.h"
#include "Project2/periodic.h"
#include "uart/uart.h"
#include "roomba/roomba.h"
#include "Project2/event.h"
#include <util/delay.h>
#include "uart/uart2.h"
#include "light_sensor/light_sensor.h"
#include "analog/analog.h"
#include "decode_joystick/decode_joystick.h"
#include "mode.h"

#define MAX_VELOCITY_DELTA_UP 150
#define MAX_VELOCITY_DELTA_DOWN 120
#define MAX_RADIUS_DELTA 50

char roomba_packet;
int drive_mode_switched = 0;

typedef struct drive_roomba_state {
	int velocity;
	int radius;
} drive_roomba_state;

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

void driveRoomba(drive_roomba_state* state) {
	roomba_packet = 0b10101111;
	joystick_values j = decode_joystick_message(roomba_packet);
	drive_values values = get_drive_values(j);
	if (drive_mode_switched) {
		gradually_come_to_stop(state);
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

//	char packet = uart2_get_byte(0);
//	uart2_reset_receive();
}

void switch_modes(void* none) {
	global_mode = 1 - global_mode;
	drive_mode_switched = 1;
}

int main() {
	DDRH = (1 << DDH3) | (1 << DDH4) | (1 << DDH5) | (1 << DDH6);
//	pan_and_tilt_init();
	disableInterrupts();
	Roomba_Init();
	_delay_ms(1000);
	enableInterrupts();
	schedulerInit(handleError);
//	addPeriodicTask(20, 100, updateLightSensorValue, 10, NULL);
	drive_roomba_state r_state = {0,0};
	addPeriodicTask(40, 200, driveRoomba, 10, &r_state);
	addPeriodicTask(5000, 5000, switch_modes, 10, NULL);
	schedulerRun();
	// Configure PORT D bit 0 to an output
//	uart2_init(UART_19200);

//	lightSensorInit();
//	for (;;) {
//		uart2_putchar(analog_read(0));
//		_delay_ms(100);
//		Roomba_Drive(20, 1);
//		_delay_ms(800);
//		Roomba_Drive(20, -1);

//	}
	return 0;
}
