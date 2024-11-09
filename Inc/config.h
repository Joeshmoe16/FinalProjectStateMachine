/*************************************************
 * AUTHOR(s): Tom Stutz, Joey Shotts
 * 
 * FILE: config.h
 * 
 * PROGRAM: 
 * 
 * DATE: (DD-MM-YYYY) 02-11-2024
 * 
 * 
 * MINIMAL HEADER
 *************************************************/


#ifndef __CONFIG_H__
#define __CONFIG_H__

///////////////////////////////////////////////////
//User Includes
///////////////////////////////////////////////////


#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stm32f446xx.h>
#include "bit.h"



///////////////////////////////////////////////////
//User Defines
///////////////////////////////////////////////////

//#define BIT(x) (1<<(x)) 

//#define PIN8_READ (GPIOB->IDR & BIT(8))
//#define PIN10_READ (GPIOB->IDR & BIT(10))

//DEBUG LED
#define PINB0_OFF (GPIOB -> ODR &= ~BIT(0))
#define PINB1_OFF (GPIOB -> ODR &= ~BIT(1))
#define PINB2_OFF (GPIOB -> ODR &= ~BIT(2))
#define PINB012_OFF (GPIOB -> ODR &= ~(BIT(0) | BIT(1) | BIT(2)))
#define PINB0_ON (GPIOB -> ODR |= BIT(0))
#define PINB1_ON (GPIOB -> ODR |= BIT(1))
#define PINB2_ON (GPIOB -> ODR |= BIT(2))

#define PINB0_TOGGLE (GPIOB -> ODR ^= BIT(0))
#define PINB1_TOGGLE (GPIOB -> ODR ^= BIT(1))
#define PINB2_TOGGLE (GPIOB -> ODR ^= BIT(2))

#define WATCHDOG_RES (IWDG->KR = 0xAAAA) /*watchdog reset*/


#define HIGH 0x00000001
#define LOW 0x00000000

#define GPIO_MODER_GPO 0x01
#define GPIO_MODER_GPI 0x00
#define GPIO_MODER_AFM 0x02
#define GPIO_MODER_AM 0x03
#define GPIO_PUPDR_NPUPD 0x00
#define GPIO_PUPDR_PU 0x01
#define GPIO_PUPDR_PD 0x02

#define GPIO_AFM0  0x0
#define GPIO_AFM1  0x1
#define GPIO_AFM2  0x2
#define GPIO_AFM3  0x3
#define GPIO_AFM4  0x4
#define GPIO_AFM5  0x5
#define GPIO_AFM6  0x6
#define GPIO_AFM7  0x7
#define GPIO_AFM8  0x8
#define GPIO_AFM9  0x9
#define GPIO_AFM10 0xA
#define GPIO_AFM11 0xB
#define GPIO_AFM12 0xC
#define GPIO_AFM13 0xD
#define GPIO_AFM14 0xE
#define GPIO_AFM15 0xF

#define EXTI_PA 0x0000
#define EXTI_PB 0x0001
#define EXTI_PC 0x0002
#define EXTI_PD 0x0003
#define EXTI_PE 0x0004
#define EXTI_PF 0x0005
#define EXTI_PG 0x0006

#define TIM_CCO 0x00
#define TIM_CCI 0x01

#define TIM_OutF 0x00   //frozen (no effect)
#define TIM_OutCA 0x01 //channel active
#define TIM_OutCL 0x02 //channel low
#define TIM_OutTOM 0x03 //toggle on match
#define TIM_OutFI 0x04 //force inactive level OCxRef is low
#define TIM_OutFA 0x05  //force active level OCxRef is active
#define TIM_OutPWM1 0x06 //pwm mode 1, active below CCR1, else low (upcount)
#define TIM_OutPWM2 0x07 //pwm mode 2, inactive below CCR1, else high (upcount)

#define TIM_InRIS 0x00 //non inverting rising edge
#define TIM_InFAL 0x01 //non inverting falling edge
#define TIM_InBE 0x05 //both edges


#define EXTI_FE 0x01
#define EXTI_RE 0x00

///////////////////////////////////////////////////
//User global var
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//User type defs
///////////////////////////////////////////////////
typedef struct
{
    GPIO_TypeDef* GPIOX;
    uint32_t PIN;
    uint32_t MODE;
    uint32_t PUPDR_STATE;
    uint32_t AFMODE;

} gpioConfig_t;

typedef struct 
{
    TIM_TypeDef* TIMX;
    uint32_t MODE;
    uint32_t CH;

    uint32_t ICxF;
    uint32_t OCxM;
    uint32_t ICxEDG;

    uint32_t CCRX;
    uint32_t ARRX;
    uint32_t PSCX;



}timConfig_t;



typedef uint32_t state_t;


///////////////////////////////////////////////////
//User fct prtype
///////////////////////////////////////////////////
void peripheral_setup(void);
void gpio_setup(void);
void watchdog_setup(void);
void SysTick_Init(void);
void SysTick_msdelay(uint32_t msdelay);

void gpio_usr_init(GPIO_TypeDef* GPIOX, uint32_t PIN, uint32_t MODE, uint32_t PUPDR_STATE, uint32_t AFMODE, gpioConfig_t* gpioStruct);
void gpio_init(gpioConfig_t* gpioStruct);
void pin_write(GPIO_TypeDef* GPIOX, uint32_t PIN, uint32_t STATE);
void pin_toggle(GPIO_TypeDef* GPIOX, uint32_t PIN);

int debug_print(const char *__restrict str, ...);

state_t pin_read(GPIO_TypeDef* GPIOX, uint32_t PIN);
state_t pin_state(GPIO_TypeDef* GPIOX, uint32_t PIN);
void timer_usr_init(TIM_TypeDef* TIMX, uint32_t MODE, uint32_t CH,uint32_t ICxF,uint32_t OCxM,uint32_t ICxEDG,uint32_t CCRX,uint32_t ARRX,uint32_t PSCX, timConfig_t* usrTim);
void timer_init(timConfig_t* usrTim);
void timer_start(TIM_TypeDef* TIMX);
void timer_stop(TIM_TypeDef* TIMX);

void interrupt_init(uint32_t EXTI_GPIOX, uint32_t PIN, uint32_t EDGE_TYPE);

#endif