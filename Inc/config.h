#ifndef config
#define config

#include "stm32f4xx.h"
//#include "stdio.h"
#include "stdint.h"

#define BIT(x) (1<<(x)) 

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

//watchdog reset
#define WATCHDOG_RES (IWDG->KR = 0xAAAA)  

void setup(void);
void gpio_setup(void);
void watchdog_setup(void);
void SysTick_Init(void);
void SysTick_msdelay(uint32_t msdelay);

#endif