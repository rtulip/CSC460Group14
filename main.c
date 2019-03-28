#define F_CPU 16000000UL

#include "Project2/scheduler.h"
#include "stdlib.h"
#include "Project2/periodic.h"
#include "uart/uart.h"
#include "roomba/roomba.h"
#include "Project2/event.h"
#include <util/delay.h>



void handleError(int error) {
	RAISE(PORTH6);
	LOWER(PORTH6);
}



void driveRoomba(void* none) {
	RAISE(PORTH4);
	int16_t v = 50;
	int16_t r = 1;
	Roomba_Drive( v, r);
	LOWER(PORTH4);
}

int main() {
	DDRH = (1 << DDH3) | (1 << DDH4) | (1 << DDH5) | (1 << DDH6);
	disableInterrupts();
	Roomba_Init();
	_delay_ms(1000);
	enableInterrupts();
	schedulerInit(handleError);

	for (;;) {
		_delay_ms(800);
		Roomba_Drive(20, 1);
		_delay_ms(800);
		Roomba_Drive(20, -1);

	}
	return 0;
}
