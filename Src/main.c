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

#include "TypicalFishImage.h"


///////////////////////////////////////////////////
//User defines
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//User global var
///////////////////////////////////////////////////
usrState_t usrState;


//////////////////////////////////////////////////
//User fct prtype
///////////////////////////////////////////////////
extern int stdout_init(void);	/* Reccomended to leave as is*/



int main(void){

	stdout_init(); /*Reccomended to leave this line as is*/
	debug_print("Hello, world!\n\r"); /*To use with UART terminal*/


	#ifndef DEV /*defined in primary.h, used for accessing UART printf at 115200 baud rate*/
	SysClockConfig(); // This line enables the clock config function to max out SPI1 clock to 45 MHZ
	#endif

	
	LCD_setup();
	peripheral_setup();

	usrState.curState = idle;
	usrState.selectedState = red;


	WATCHDOG_RES;
	for(;;)
	{
		
		WATCHDOG_RES;
		SysTick_msdelay(500);
		
		switch(usrState.curState)
		{
			case idle:
				WATCHDOG_RES;
				
				// Draw_String_BG(0,linspace*1,"Idle  ",WHITE,BLACK,&font_ubuntu_mono_24);
				poll_encoder();
				poll_encoder_press();
				
				break;
			case red:
				WATCHDOG_RES;
				pin_write(DBG_LD1_PORT,DBG_LD1,HIGH);
				pin_write(DBG_LD2_PORT,DBG_LD2,LOW);
				pin_write(DBG_LD3_PORT,DBG_LD3,LOW);
				// Draw_String_BG(0,linspace*1,"RED   ",RED,BLACK,&font_ubuntu_mono_24);

				usrState.curState = idle;

				break;
			case green:
				WATCHDOG_RES;
				pin_write(DBG_LD1_PORT,DBG_LD1,LOW);
				pin_write(DBG_LD2_PORT,DBG_LD2,HIGH);
				pin_write(DBG_LD3_PORT,DBG_LD3,LOW);
				// Draw_String_BG(0,linspace*1,"GREEN",GREEN,BLACK,&font_ubuntu_mono_24);
				
				usrState.curState = idle;

				break;
			case blue:
				WATCHDOG_RES;
				pin_write(DBG_LD1_PORT,DBG_LD1,LOW);
				pin_write(DBG_LD2_PORT,DBG_LD2,LOW);
				pin_write(DBG_LD3_PORT,DBG_LD3,HIGH);
				// Draw_String_BG(0,linspace*1,"BLUE  ",BLUE,BLACK,&font_ubuntu_mono_24);

				usrState.curState = idle;

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

	DrawPaletteImage(0, 0, TypicalFish);
	SysTick_msdelay(1000);

    Fill_Screen(BLACK);

	Draw_String_BG(0,0,"EGR 326 Final Project", WHITE,BLACK,&font_ubuntu_mono_24);
	Draw_Char_BG(TFT_WIDTH-24,TFT_HEIGHT-24,'C',BLUE,BLACK,&font_ubuntu_mono_24);
	debug_print("Init Complete\n\r");

}

/*******
FUNCTION: debounce_release
INPUTS: 
OUTPUTS: 
DESCRIPTION: debounce for button release
*******/
uint64_t debounce_release(GPIO_TypeDef* GPIOX, uint32_t PIN)
{
	static uint64_t state = 0;
	WATCHDOG_RES;
	state = (state << 1) | (GPIOX->IDR & (1<<PIN))>>PIN;
	if(state == 0xffffffffffffffff)
	{
		state = 0;
		return 1;

	}

	return 0;

}

/*******
FUNCTION: poll_encoder
INPUTS: 
OUTPUTS: 
DESCRIPTION: 
*******/
void poll_encoder(void)
{
	if(!pin_read(ROTARY_CLK_PORT,ROTARY_CLK))
	{
		if(pin_read(ROTARY_DT_PORT,ROTARY_DT))
		{
			Draw_Box(0,linspace*usrState.selectedState,70,20,2,BLACK);
			usrState.selectedState++;
			if(usrState.selectedState > 3)
			{
				usrState.selectedState = 1;
			}
		}
		else
		{
			Draw_Box(0,linspace*usrState.selectedState,70,20,2,BLACK);
			usrState.selectedState--;
			if(usrState.selectedState < 1)
			{
				usrState.selectedState = 3;
			}
		}
		Draw_Box(0,linspace*usrState.selectedState,70,20,2,WHITE);
		while(!debounce_release(ROTARY_CLK_PORT,ROTARY_CLK)){};
		

	}

}


/*******
FUNCTION: poll_encoder_press
INPUTS: 
OUTPUTS: 
DESCRIPTION: 
*******/
void poll_encoder_press(void)
{
	if(!pin_read(ROTARY_SW_PORT,ROTARY_SW))
	{
		while(!debounce_release(ROTARY_SW_PORT,ROTARY_SW)){};
		usrState.curState = usrState.selectedState;

	}

}





/*EOF*/
