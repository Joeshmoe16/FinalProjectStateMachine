/*************************************************
 * AUTHOR(s): Tom Stutz
 * 
 * FILE: spi.c
 * 
 * PROGRAM: 
 * 
 * DESCRIPTION: 
 * 
 * VERSION: 1.0
 * 
 * DATE: (DD-MM-YYYY) 29-08-24
 * 
 * 
 * VERBOSE HEADER
 *************************************************/


///////////////////////////////////////////////////
//User includes
///////////////////////////////////////////////////
#include "spi.h"
///////////////////////////////////////////////////
//User defines
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//User global var
///////////////////////////////////////////////////
gpioConfig_t spiGpio;

///////////////////////////////////////////////////
//User fct defs
///////////////////////////////////////////////////




/// @brief Initialize SPI1 registers for STM32F446RE Nucleo board
void SPI_Init(void)
{
//initialize spi registers
    SPI1->CR1 &= ~(0x1 << 6);  
//bit 12 of apb2
    RCC->APB2ENR |= 0x01 << 12;
//1 to bit 14, 1 to bit 7, 1 to bit 2
    SPI1->CR1 |= (0x01 << 15) | (0x01 << 14) | (0x01 << 2);
    SPI1->CR2 |= (0x01<<7) | (0x01 << 2) ;
    SPI_GPIO();

    SPI1->CR1 |= 0x1 << 6;
    

}

/// @brief Enable GPIO for SPI1 on STM32F446RE Nucleo board
void SPI_GPIO(void)
{
    //GPIO init for SPI registers
    //MOSI
    gpio_usr_init(MOSI_GPIO,MOSI_PIN,GPIO_MODER_AFM,GPIO_PUPDR_NPUPD,GPIO_AFM5,&spiGpio);
    //MISO
    gpio_usr_init(MISO_GPIO,MISO_PIN,GPIO_MODER_AFM,GPIO_PUPDR_NPUPD,GPIO_AFM5,&spiGpio);

    //SCK
    gpio_usr_init(SCK_GPIO,SCK_PIN,GPIO_MODER_AFM,GPIO_PUPDR_NPUPD,GPIO_AFM5,&spiGpio);

    //CS
    gpio_usr_init(CS_GPIO,CS_PIN,GPIO_MODER_GPO,GPIO_PUPDR_NPUPD,0x00,&spiGpio);

    //RESET
    gpio_usr_init(LCD_RST_PORT,LCD_RST,GPIO_MODER_GPO,GPIO_PUPDR_NPUPD,0x00,&spiGpio);

    //DCX
    gpio_usr_init(LCD_DCX_PORT,LCD_DCX,GPIO_MODER_GPO,GPIO_PUPDR_NPUPD,0x00,&spiGpio);


}

void SPI_Disable(void)
{
   //disable SPI in CR1 register
}

void SPI_Enable(void)
{
   // enable SPI in CR1 register
}

void SPI_Transmit(uint8_t *data, uint8_t size)
{
    uint8_t i = 0;
    while (i < size)
    {
        while (!(SPI1->SR & 0x2))
        {
        }
        SPI1->DR = data[i];
        i++;
    }

    while (!(SPI1->SR & 0x2))
    {
    }
    while (SPI1->SR & 0x80)
    {
    }

    uint8_t temp = SPI1->DR;
    temp = SPI1->SR;
}


void CS_Low(void)
{
    CS_GPIO->ODR &= ~(1 << CS_PIN);
}

void CS_High(void)
{
    CS_GPIO->ODR |= 1 << CS_PIN;
}




//EOF



