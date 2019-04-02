
#define F_CPU 16000000UL

#include "Project2/scheduler.h"
#include "stdlib.h"
#include "Project2/periodic.h"
#include "uart/uart.h"
#include <avr/io.h>
#include "Project2/event.h"
#include <util/delay.h>
#include "uart/uart2.h"
#include "analog/analog.h"
#include "joystick/joystick.h"
#include "mode.h"


char roomba_packet;


void handleError(int error) {
	RAISE(PORTH6);
	LOWER(PORTH6);
}

void sendJoystickPackets(void* none) {
	unsigned char roomba;
	unsigned char servo;
	createPackets(&roomba, &servo);
	uart2_putchar(roomba);
	uart2_putchar(servo);
}



int main() {
	DDRH = (1 << DDH3) | (1 << DDH4) | (1 << DDH5) | (1 << DDH6);
	// Set analog pins as input for joysticks.
	uart2_init(UART_9600);
	enableInterrupts();
	schedulerInit(handleError);
	setupJoysticks();
	addPeriodicTask(0, 100, sendJoystickPackets, 10, NULL);
	schedulerRun();
	return 0;
}
