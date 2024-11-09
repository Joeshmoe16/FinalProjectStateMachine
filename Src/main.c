/*************************************************
 * AUTHOR(s): Joey Shotts, Tom Stutz
 * 
 * FILE: main.c
 * 
 * PROGRAM: EGR_326_FinalProject 
 * 
 * DESCRIPTION: 
 * 
 * VERSION: 1.1.0
 * 
 * DATE: (DD-MM-YYYY) 31-10-2024
 * 
 * 
 * VERBOSE HEADER
 *************************************************/


///////////////////////////////////////////////////
//User Includes
///////////////////////////////////////////////////
#define BIT(x) (1<<(x))
#include "config.h"
#include "stm32f4xx.h"
#include "stdint.h"
#include "main.h"

/*LCD includes (Reccomended to leave in main for now)*/
#include "ILI9341.h"
#include "RccConfig.h"
#include "font_ubuntu_mono_24.h"
#include "font_freemono_mono_bold_24.h"
#include "font_firacode_regular_14.h"
#include "font_fixedsys_mono_16.h"
#include "font_fixedsys_mono_24.h"
#include "font_freemono_mono_24.h"
#include "my_photos.h"
#include "spi.h"



///////////////////////////////////////////////////
//User defines
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//User global var
///////////////////////////////////////////////////
enum states {
	idle,
	red,
	green,
	blue,
};

//////////////////////////////////////////////////
//User fct prtype
///////////////////////////////////////////////////
extern int stdout_init(void);	/* Reccomended to leave as is*/



int main(void){

	stdout_init(); /*Reccomended to leave this line as is*/
	debug_print("Hello, world!\n\r"); /*To use with UART terminal*/


	
	SysClockConfig(); // This line enables the clock config function to max out SPI1 clock to 45 MHZ

	enum states curState = idle;
	LCD_setup();
	peripheral_setup();
	WATCHDOG_RES;
	for(;;)
	{
		switch(curState)
		{
			case idle:
				WATCHDOG_RES;
				PINB012_OFF;
				SysTick_msdelay(250);
				Draw_String_BG(0,linspace*1,"Idle  ",WHITE,BLACK,&font_ubuntu_mono_24);
				curState = red;
				break;
			case red:
				WATCHDOG_RES;
				PINB012_OFF;
				PINB0_ON;
				SysTick_msdelay(250);
				Draw_String_BG(0,linspace*1,"RED   ",RED,BLACK,&font_ubuntu_mono_24);
				curState = green;
				break;
			case green:
				WATCHDOG_RES;
				PINB012_OFF;
				PINB1_ON;
				SysTick_msdelay(250);
				Draw_String_BG(0,linspace*1,"GREEN",GREEN,BLACK,&font_ubuntu_mono_24);
				curState = blue;	
				break;
			case blue:
				WATCHDOG_RES;
				PINB012_OFF;
				PINB1_ON;
				SysTick_msdelay(250);
				Draw_String_BG(0,linspace*1,"BLUE  ",BLUE,BLACK,&font_ubuntu_mono_24);
				curState = idle;
				break;
			default:
				break;
		}
	}
}

///////////////////////////////////////////////////
//User fct defs
///////////////////////////////////////////////////
/*******
FUNCTION: LCD_setup();
INPUTS: 
OUTPUTS: N/A
DESCRIPTION: This function initializes i2c 1
*******/
void LCD_setup(void)
{
	//SysClockConfig(); // This line enables the clock config function to max out SPI1 clock to 45 MHZ
    Display_Init(); // Initializes the LCD
	Rotate_Display(LANDSCAPE);

    Fill_Screen(BLACK);

	Draw_String_BG(0,0,"EGR 326 Final Project", WHITE,BLACK,&font_ubuntu_mono_24);
	Draw_Char_BG(TFT_WIDTH-24,TFT_HEIGHT-24,'C',BLUE,BLACK,&font_ubuntu_mono_24);
	debug_print("Init Complete\n\r");

}





/*EOF*/
