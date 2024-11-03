/*  Author: Jacob Clarey
 *  Date: 8/14/2023
 *  Description: A header to contain function relating to
 *  system clock configuration settings for the F446RE
 *  Notes: Using 8MHz Crystal as HSE
 */

#ifndef __RCCCONFIG_H__
#define __RCCCONFIG_H__

#include <stm32f446xx.h>

/********************************************************************************
Equation Sheet for Clock Sourcing:

HSE / PLL_M * PLL_N / PLL_P = SYSCLK (SysTick Clock) (limitless ?)
SYSCLK / AHB_PSC = HCLK (AHB1ENR Clock) (180 MHz maximum)

HCLK / APB1_PSC = PCLK1 (APB1ENR Clock) (45 MHz maximum)
PCLK1 * 2 = APB1_Timer_Clocks (90 MHz maximum)

HCLK / APB2_PSC = PCLK2 (APB2ENR Clock) (90 MHz maximum)
PCLK2 * 2 = APB2_Timer_Clocks (180 MHz maximum)
********************************************************************************/

/*******************************************************************************/
/*                            USER PREPROCESSOR DEFS */
/*******************************************************************************/
/*
1: 0b0xxx   0
2: 0b1000   8
4: 0b1001   9
8: 0b1010   10
16: 0b1011  11
64: 0b1100  12
128: 0b1101 13
256: 0b1110 14
512: 0b1111 15
*/
/// @brief AHB prescaler value
#define AHB_PSC 0

/*
1: 0b0xx  0
2: 0b100  4
4: 0b101  5
8: 0b110  6
16: 0b111 7
*/
/// @brief APB1 prescaler value
#define APB1_PSC 4
/// @brief APB2 prescaler value
#define APB2_PSC 0

/*
2: 0b00 0
4: 0b01 1
6: 0b10 2
8: 0b11 4
*/
/// @brief PLL_P division value
#define PLL_P 2

/* direct conversion for these two values */
/// @brief PLL_M division value
#define PLL_M 4
/// @brief PLL_N multiplier value
#define PLL_N 90

/*******************************************************************************/
/*******************************************************************************/

/*******************************************************************************/
/*                           USER FUNCTION PROTOTYPE */
/*******************************************************************************/
/// @brief Enables the SysClock to be at desired frequency.
/// @brief Without calling this, the default is 16MHz using
/// @brief the internal RC oscillator.
void SysClockConfig(void);
/*******************************************************************************/
/*******************************************************************************/

#endif //__RCCCONFIG_H__

/* EOF */