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
#include "i2c.h"



///////////////////////////////////////////////////
//User Defines
///////////////////////////////////////////////////
/*comment out for faster LCD printing*/
#define DEV
/*comment out if RTC has correct time*/
//#define TIME_INIT

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


//RTC defines
#define RTC_ADDR 0x68
#define RTC_Seconds 0x00
#define RTC_Minutes 0x01
#define RTC_Hours 0x02
#define RTC_Day 0x03
#define RTC_Date 0x04
#define RTC_Month 0x05
#define RTC_Year 0x06

#define RTC_TempInt 0x11
#define RTC_TempFrac 0x12
#define RTC_Control 0x0E
#define RTC_Status 0x0F


//RTC translate
#define MON 0x01
#define TUE 0x02
#define WED 0x03
#define THU 0x04
#define FRI 0x05
#define SAT 0x06
#define SUN 0x07

#define JAN 0x01
#define FEB 0x02
#define MAR 0x03
#define APR 0x04
#define MAY 0x05
#define JUN 0x06
#define JUL 0x07
#define AUG 0x08
#define SEP 0x09
#define OCT 0x10
#define NOV 0x11
#define DEC 0x12

#define RTC_interrupt_PORT GPIOB
#define RTC_interrupt 7

#define TIME_X_POS TFT_WIDTH-90
#define TIME_Y_POS 0


#define BIT(x) (1<<(x))





///////////////////////////////////////////////////
//User global var
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//User type defs
///////////////////////////////////////////////////
typedef struct 
{
    char seconds[2];
    char minutes[2];
    char hours[2];
    char day[2];
    char date[2];
    char month[2];
    char year[2];
   
}RTC_t;



///////////////////////////////////////////////////
//User fct prtype
///////////////////////////////////////////////////


#endif

/*EOF*/
