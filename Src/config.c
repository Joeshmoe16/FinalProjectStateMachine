/*******************************************************************************
* Name:             Joseph Shotts
* Course:           EGR 326 Embedded System Design
* Project:          DemoProject
* File:             config.c
* Description:      Includes functions to initialize GPIO pins and systick.
********************************************************************************/

#include "config.h"

/***| setup() |*****************************//*
*Brief: 
*	Runs Setup Functions
*Params:
*            None
*Returns:
*           None
**********************************************/
void setup(void){
	gpio_setup();
	SysTick_Init();
	watchdog_setup();
}

/***| gpio_setup() |*****************************//*
*Brief: 
*	Initializes LEDs and Buttons
*Params:
*            None
*Returns:
*           None
********************************************************/
void gpio_setup(void)
{
	//Enable GPIO B
	RCC -> AHB1ENR |= BIT(1);

	//MODERx[0:1} (x is pins 0, 2, 4, 6)
	GPIOB -> MODER |= BIT(0);
	GPIOB -> MODER |= BIT(2);
	GPIOB -> MODER |= BIT(4);
	
	//Turn all LEDs Off
	PINB012_OFF;
	
	/*
	//MODER8[0:0] button pin is 8, add a pull up resistor using PUPDR[1:1]
	GPIOB -> MODER &= ~(BIT(16)|BIT(17));
	GPIOB -> PUPDR  |= BIT(16);
	
	//MODER6[0:0] button pin is 6, add a pull up resistor using PUPDR[1:1]
	GPIOB -> MODER &= ~(BIT(20)|BIT(21));
	GPIOB -> PUPDR  |= BIT(20);
	*/
}

/***| watchdog_setup() |*****************************//*
*Brief: 
*	Initializes and resets Systick
*Params:
*            None
*Returns:
*           None
********************************************************/
void watchdog_setup(void){

	IWDG->KR = 0x0000;      //reset KR register
	IWDG->KR = 0x5555;      //enable watchdog register access
	IWDG->PR = 0x04;        //2ms-8192ms
	while(!(IWDG->SR | 0x01)); //wait for PR register to update
	IWDG->RLR = 2000 - 1;   //11 bits, 0-2047, with 0x04 PSC: 2ms/val 
	while(!(IWDG->SR | 0x02)); //wait for RLR register to update
	IWDG->KR = 0xCCCC;      //watchdog start
	IWDG->KR = 0xAAAA;      //watchdog reset
}

/***| Systick_Init() |*****************************//*
*Brief: 
*	Initializes and resets Systick
*Params:
*            None
*Returns:
*           None
********************************************************/
void SysTick_Init (void)
{                                                       // initialization of SysTick timer
    SysTick -> CTRL     = 0;                            // disable SysTick during step
    SysTick -> LOAD     = 0x00FFFFFF;                   // max reload value
    SysTick -> VAL      = 0;                            // any write to current clears it
    SysTick -> CTRL     = 0x00000005;                   // enable SysTick, 16MHz, No Interrupts
}

/***| msDelay(uint16_t n) |*****************************//*
*Brief: msDelay Function, generates a delay using a for loop
*				the delays are not timed, eye balled for debouncing
*Params:
*           uint16_t n
*Returns:
*           None
********************************************************/
void SysTick_msdelay (uint32_t msdelay)                 // SysTick delay function
{
	SysTick -> LOAD = ((msdelay * 16000) - 1);          // delay for 1 ms* delay value
	SysTick -> VAL  = 0;                                // any write to CVR clears it

	while (!(SysTick -> CTRL  &  0x00010000));
	
}


/***|debounce(uint16_t delay) |*****************************//*
*Brief: 
*	Denounces the button: If button is on after ~10 cycles return true. 
* If button is off after ~10 cycles return false. 
*Params:
*          
*Returns:
*           True/False if button is on
********************************************************/
/*unsigned char debounce(void)
{
	static uint32_t State = 0; // Current debounce status
	static char output = 0;   //output value, stored for debouncing release
	
	//checks if button was previously off
	if(!output){
		// read switch, upper 3 bits of State are don't cares
		State=(State<<1) | PINB6_READ | 0xe0000000; 
		if (State == 0xf0000000) {output = 1; State = 0;}
		else output = 0;
	}
	else{
		// read switch, upper 3 bits of State are don't cares
		State=(State<<1) | !PINB6_READ | 0xe0000000; 
		if (State == 0xf0000000) {output = 0; State = 0;}
		else output = 1;
	}
	
	return output;
}
*/
