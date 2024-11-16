/*************************************************
 * AUTHOR(s): Tom Stutz
 * 
 * FILE: main.h
 * 
 * PROGRAM: 
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

#ifndef __MAIN_H__
#define __MAIN_H__

///////////////////////////////////////////////////
//User Includes
///////////////////////////////////////////////////
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f446xx.h"
#include "i2c.h"
#include "primary.h"



///////////////////////////////////////////////////
//User Defines
///////////////////////////////////////////////////

#define GPIO_MODER_GPO 0x01
#define GPIO_MODER_GPI 0x00
#define GPIO_MODER_AFM 0x02
#define GPIO_MODER_AM 0x03

//debug defines
#define MAX_CHAR_PRINT 128 /*Reccomended to leave as is*/

#define BTN1 4 /*white button, forward button, PA4*/
#define BTN2 0 /*red button, reverse button, PA6*/

#define MICRO_SLAVE_ADDR 0x5A

#define linspace 30
#define INVALID_LIN 290




///////////////////////////////////////////////////
//User global var
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//User type defs
///////////////////////////////////////////////////

typedef struct
{
   bool btn1Flag;
   bool btn2Flag;
   
} button_t;

enum states_t
{
	idle,
	red,
	green,
	blue,
};


typedef struct
{
   enum states_t curState;
   enum states_t selectedState;
  
} usrState_t;




///////////////////////////////////////////////////
//User fct prtype
///////////////////////////////////////////////////
uint64_t debounce_release(GPIO_TypeDef* GPIOX, uint32_t PIN);
void LCD_setup(void);
void poll_encoder(void);
void poll_encoder_press(void);
void init_LCD_clock(void);

extern void EXTI9_5_IRQHandler(void);






#endif


//EOF
