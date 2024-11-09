/*************************************************
 * AUTHOR(s): Tom Stutz, Joey Shotts
 * 
 * FILE: primary.h
 * 
 * PROGRAM: 
 * 
 * DATE: (DD-MM-YYYY) 02-11-2024
 * 
 * 
 * MINIMAL HEADER
 *************************************************/

#ifndef __PRIMARY_H__
#define __PRIMARY_H__


///////////////////////////////////////////////////
//User Includes
///////////////////////////////////////////////////



///////////////////////////////////////////////////
//User Defines
///////////////////////////////////////////////////
/*comment out for faster LCD printing*/
//#define DEV

#define DBG_LD1_PORT GPIOB
#define DBG_LD2_PORT GPIOB
#define DBG_LD3_PORT GPIOB
#define DBG_LD1 2
#define DBG_LD2 3
#define DBG_LD3 4

#define USR_BTN_PORT GPIOC
#define USR_BTN 12

#define ROTARY_CLK_PORT GPIOB
#define ROTARY_DT_PORT GPIOB
#define ROTARY_SW_PORT GPIOC
#define ROTARY_CLK 10
#define ROTARY_DT 12
#define ROTARY_SW 13

#define BIT(x) (1<<(x))

///////////////////////////////////////////////////
//User global var
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//User type defs
///////////////////////////////////////////////////



///////////////////////////////////////////////////
//User fct prtype
///////////////////////////////////////////////////


#endif

/*EOF*/
