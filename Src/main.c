#include "config.h"
#include "stm32f4xx.h"
#include "stdint.h"

enum states {
	idle,
	red,
	green,
	blue,
};

int main(void){
	enum states curState = idle;

	setup();
	WATCHDOG_RES;
	for(;;){
		switch(curState){
			case idle:
				WATCHDOG_RES;
				PINB012_OFF;
				SysTick_msdelay(250);
				curState = red;
				break;
			case red:
				WATCHDOG_RES;
				PINB012_OFF;
				PINB0_ON;
				SysTick_msdelay(250);
				curState = green;
				break;
			case green:
				WATCHDOG_RES;
				PINB012_OFF;
				PINB1_ON;
				SysTick_msdelay(250);
				curState = blue;	
				break;
			case blue:
				WATCHDOG_RES;
				PINB012_OFF;
				PINB1_ON;
				SysTick_msdelay(250);
				curState = idle;
				break;
			default:
				break;
		}
	}
}