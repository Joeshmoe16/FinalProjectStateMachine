/*************************************************
 * AUTHOR(s): Tom Stutz, Joey Shotts
 * 
 * FILE: config.c
 * 
 * PROGRAM: 
 * 
 * DATE: (DD-MM-YYYY) 02-11-2024
 * 
 * 
 * MINIMAL HEADER
 *************************************************/
#include "config.h"



gpioConfig_t gpioStructConfig;

/***| setup() |*****************************//*
*Brief: 
*	Runs Setup Functions
*Params:
*            None
*Returns:
*           None
**********************************************/
void peripheral_setup(void){

	gpio_setup();
	SysTick_Init();
	watchdog_setup();
	debug_LED_setup();
	usr_button_setup();
	rotary_encoder_setup();
	RTC_init();
	
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

/***| debug_LED_setup() |*****************************//*
*Brief: 
*	Initializes debug LED
*Params:
*            None
*Returns:
*           None
********************************************************/
void debug_LED_setup(void)
{
	gpio_usr_init(DBG_LD1_PORT,DBG_LD1,GPIO_MODER_GPO,0x00,0x00,&gpioStructConfig);
	gpio_usr_init(DBG_LD2_PORT,DBG_LD2,GPIO_MODER_GPO,0x00,0x00,&gpioStructConfig);
	gpio_usr_init(DBG_LD3_PORT,DBG_LD3,GPIO_MODER_GPO,0x00,0x00,&gpioStructConfig);

}

/***| usr_button_setup() |*****************************//*
*Brief: 
*	Initializes user button
*Params:
*            None
*Returns:
*           None
********************************************************/
void usr_button_setup(void)
{
	gpio_usr_init(USR_BTN_PORT,USR_BTN,GPIO_MODER_GPI,0x00,0x00,&gpioStructConfig);

}

/***| rotary_encoder_setup() |*****************************//*
*Brief: 
*	Initializes user button
*Params:
*            None
*Returns:
*           None
********************************************************/
void rotary_encoder_setup(void)
{
	gpio_usr_init(ROTARY_CLK_PORT,ROTARY_CLK,GPIO_MODER_GPI,GPIO_PUPDR_NPUPD,0x00,&gpioStructConfig);
	gpio_usr_init(ROTARY_DT_PORT,ROTARY_DT,GPIO_MODER_GPI,GPIO_PUPDR_NPUPD,0x00,&gpioStructConfig);
	gpio_usr_init(ROTARY_SW_PORT,ROTARY_SW,GPIO_MODER_GPI,GPIO_PUPDR_NPUPD,0x00,&gpioStructConfig);

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

/*******
FUNCTION: gpio_usr_init
INPUTS: 
OUTPUTS: 
DESCRIPTION: user specifies the gpio information for initialization
*******/
void gpio_usr_init(GPIO_TypeDef* GPIOX, uint32_t PIN, uint32_t MODE, uint32_t PUPDR_STATE, uint32_t AFMODE, gpioConfig_t* gpioStruct)
{

    gpioStruct->GPIOX = GPIOX;
    gpioStruct->PIN = PIN;
    gpioStruct->MODE = MODE;
    gpioStruct->PUPDR_STATE = PUPDR_STATE;
	gpioStruct->AFMODE = AFMODE;
    gpio_init(gpioStruct);

}

/*******
FUNCTION: gpio_setup
INPUTS: 
OUTPUTS: 
DESCRIPTION: write the gpio registers with the given gpio structure
*******/
void gpio_init(gpioConfig_t* gpioStruct) 
{
	
	//enable clock to RCC for given GPIO
    RCC->AHB1ENR |= 0x07;
		
    
	//clear mode register
	gpioStruct->GPIOX->MODER &= ~(0x03 <<(2*gpioStruct->PIN));
	
	//GPIOA->MODER write | 0b 0100 0000 0000 (Make MODER5 01 General purpose output mode)
    gpioStruct->GPIOX->MODER |= (gpioStruct->MODE << (2*gpioStruct->PIN));

    gpioStruct->GPIOX->PUPDR |= (gpioStruct->PUPDR_STATE << (2*gpioStruct->PIN));

	if(gpioStruct->MODE == GPIO_MODER_AFM)
	{
		switch(gpioStruct->AFMODE)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			if(gpioStruct->PIN < 8)
			{
				gpioStruct->GPIOX->AFR[0] |= gpioStruct->AFMODE << (4*gpioStruct->PIN);
			}
			else
			{
				gpioStruct->GPIOX->AFR[1] |= gpioStruct->AFMODE << (4*(gpioStruct->PIN - 8));
			}
			
			break;
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			if(gpioStruct->PIN < 8)
			{
				gpioStruct->GPIOX->AFR[0] |= gpioStruct->AFMODE << (4*gpioStruct->PIN);
			}
			else
			{
				gpioStruct->GPIOX->AFR[1] |= gpioStruct->AFMODE << (4*(gpioStruct->PIN - 8));
			}
			
			break;
			default:
			break;
		}
	}
	
}


/*******
FUNCTION: pin_write
INPUTS: 
OUTPUTS: 
DESCRIPTION: write a 1 or a 0 to ODR of the given GPIO pin
*******/
void pin_write(GPIO_TypeDef* GPIOX, uint32_t PIN, uint32_t STATE)
{

    if(!((STATE != HIGH) || (STATE != LOW)))
    {
        //ERROR in function
    }
    else
    {
        //Clear pin
        GPIOX->ODR &= ~(0x01 << (PIN));

        //set with state
        switch (STATE)
        {

            case LOW:
            GPIOX->ODR |= (0x00 << (PIN));
            break;
            case HIGH:
            GPIOX->ODR |= (0x01 <<(PIN));
            break;

        }
    }

}

/*******
FUNCTION: pin_toggle
INPUTS: 
OUTPUTS: 
DESCRIPTION: toggles user specified pin
*******/
void pin_toggle(GPIO_TypeDef* GPIOX, uint32_t PIN)
{
    GPIOX->ODR ^= (0x01 << (PIN));
}



/*******
FUNCTION: pin_read
INPUTS: 
OUTPUTS: 
DESCRIPTION: returns the state of the pin (HIGH or LOW)
*******/
state_t pin_read(GPIO_TypeDef* GPIOX, uint32_t PIN)
{
    return (GPIOX->IDR & (0x01 << PIN));
}

/*******
FUNCTION: pin_state
INPUTS: 
OUTPUTS: 
DESCRIPTION: returns the state of the pin from ODR(HIGH or LOW)
*******/
state_t pin_state(GPIO_TypeDef* GPIOX, uint32_t PIN)
{
    return (GPIOX->ODR & (0x01 << PIN));
}


/*******
FUNCTION: interrupt_init
INPUTS: 
OUTPUTS: 
DESCRIPTION: initializes the interrupt for EXTI
*******/
void interrupt_init(uint32_t EXTI_GPIOX, uint32_t PIN, uint32_t EDGE_TYPE) {
	
	__disable_irq();
	RCC->APB2ENR |= (0x01 << 14);

//configure the EXTICR with the desired gpio
if(PIN < 4)
{
	SYSCFG->EXTICR[0] &= ~(0x0F << (4*PIN));
	SYSCFG->EXTICR[0] |=  (EXTI_GPIOX << (4*PIN));

}
else if(PIN < 8)
{
	SYSCFG->EXTICR[1] &= ~(0x0F << (4*(PIN-4)));
	SYSCFG->EXTICR[1] |=  (EXTI_GPIOX << (4*(PIN-4)));

}
else if(PIN < 12)
{
	SYSCFG->EXTICR[2] &= ~(0x0F << (4*PIN));
	SYSCFG->EXTICR[2] |=  (EXTI_GPIOX << (4*(PIN-8)));

}
else if(PIN < 16)
{
	SYSCFG->EXTICR[3] &= ~(0x0F << (4*(PIN-12)));
	SYSCFG->EXTICR[3] |=  (EXTI_GPIOX << (4*(PIN-12)));

}
else
{
	__NOP(); //bad logic
}

//configure the IMR
EXTI->IMR |= (0x01 << PIN);

//configure the edge type
if(EDGE_TYPE == EXTI_FE)
{
	EXTI->FTSR |= (0x01 << PIN);
	EXTI->RTSR &= ~(0x01 << PIN);
}
else if(EDGE_TYPE == EXTI_RE)
{
	EXTI->FTSR &= ~(0x01 << PIN);
	EXTI->RTSR |= (0x01 << PIN);
}
else 
{
	__NOP();
}

//enable the correct interrupt line
switch(PIN)
{
	case 0:
	NVIC_EnableIRQ(EXTI0_IRQn);
	break;
	case 1:
	NVIC_EnableIRQ(EXTI1_IRQn);
	break;
	case 2:
	NVIC_EnableIRQ(EXTI2_IRQn);
	break;
	case 3:
	NVIC_EnableIRQ(EXTI3_IRQn);
	break;
	case 4:
	NVIC_EnableIRQ(EXTI4_IRQn);
	break;

	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	break;

	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	break;

}


  __enable_irq();

}

/*******
FUNCTION: timer init
INPUTS: 
OUTPUTS: 
DESCRIPTION: initializes the timer
*******/
void timer_init(timConfig_t* usrTim)
{
	//turn on RCC for timer
	if(usrTim->TIMX == TIM1)
	{
		RCC->APB2ENR |= 0x01 << 0;
	}
	else if(usrTim->TIMX == TIM2)
	{
		RCC->APB1ENR |= 0x01 << 0;
	}
	else if(usrTim->TIMX == TIM3)
	{
		RCC->APB1ENR |= 0x01 << 1;
	}
	else if(usrTim->TIMX == TIM4)
	{
		RCC->APB1ENR |= 0x01 << 2;
	}
	else if(usrTim->TIMX == TIM5)
	{
		RCC->APB1ENR |= 0x01 << 3;
	}
	else if(usrTim->TIMX == TIM6)
	{
		RCC->APB1ENR |= 0x01 << 4;
	}
	else if(usrTim->TIMX == TIM7)
	{
		RCC->APB1ENR |= 0x01 << 5;
	}
	else if(usrTim->TIMX == TIM8)
	{
		RCC->APB2ENR |= 0x01 << 1;
	}
	else if(usrTim->TIMX == TIM9)
	{
		RCC->APB2ENR |= 0x01 << 16;
	}
	else if(usrTim->TIMX == TIM10)
	{
		RCC->APB2ENR |= 0x01 << 17;
	}
	else if(usrTim->TIMX == TIM11)
	{
		RCC->APB2ENR |= 0x01 << 18;
	}
	else if(usrTim->TIMX == TIM12)
	{
		RCC->APB1ENR |= 0x01 << 6;
	}
	else if(usrTim->TIMX == TIM13)
	{
		RCC->APB1ENR |= 0x01 << 7;
	}
	else if(usrTim->TIMX == TIM14)
	{
		RCC->APB1ENR |= 0x01 << 8;
	}
	//set the mode for the given channel
	switch(usrTim->CH)
	{
		case 1:
			usrTim->TIMX->CCMR1 |= usrTim->MODE << 0;
			if(usrTim->MODE == TIM_CCI)
			{
				usrTim->TIMX->CCMR1 |= usrTim->ICxF << 4;
				usrTim->TIMX->CCER |= usrTim->ICxEDG << 1;
			}
			else
			{
				usrTim->TIMX->CCMR1 |= usrTim->OCxM << 4;
			}
			usrTim->TIMX->CCER |= 0x01 << 0;
			usrTim->TIMX->CCR1 = usrTim->CCRX;
		break;
		case 2:
			usrTim->TIMX->CCMR1 |= usrTim->MODE << 8;
			if(usrTim->MODE == TIM_CCI)
			{
				usrTim->TIMX->CCMR1 |= usrTim->ICxF << 12;
				usrTim->TIMX->CCER |= usrTim->ICxEDG << 5;
			}
			else
			{
				usrTim->TIMX->CCMR1 |= usrTim->OCxM << 8;
			}
			usrTim->TIMX->CCER |= 0x01 << 4;
			usrTim->TIMX->CCR2 = usrTim->CCRX;
		break;
		case 3:
			usrTim->TIMX->CCMR2 |= usrTim->MODE << 0;
			if(usrTim->MODE == TIM_CCI)
			{
				usrTim->TIMX->CCMR2 |= usrTim->ICxF << 4;
				usrTim->TIMX->CCER |= usrTim->ICxEDG << 9;
			}
			else
			{
				usrTim->TIMX->CCMR2 |= usrTim->OCxM << 4;
			}
			usrTim->TIMX->CCER |= 0x01 << 8;
			usrTim->TIMX->CCR3 = usrTim->CCRX;
		break;
		case 4:
			usrTim->TIMX->CCMR2 |= usrTim->MODE << 8;
			if(usrTim->MODE == TIM_CCI)
			{
				usrTim->TIMX->CCMR2 |= usrTim->ICxF << 12;
				usrTim->TIMX->CCER |= usrTim->ICxEDG << 13;
			}
			else
			{
				usrTim->TIMX->CCMR2 |= usrTim->OCxM << 8;
			}
			usrTim->TIMX->CCER |= 0x01 << 12;
			usrTim->TIMX->CCR4 = usrTim->CCRX;
		break;
		default:
		break;
	}
	//set arr and psc
	usrTim->TIMX->PSC = usrTim->PSCX;
	usrTim->TIMX->ARR = usrTim->ARRX;



}
/*******
FUNCTION: timer usr init
INPUTS: 
OUTPUTS: 
DESCRIPTION: initializes timer with user inputs
*******/
void timer_usr_init(TIM_TypeDef* TIMX, uint32_t MODE, uint32_t CH, uint32_t ICxF, uint32_t OCxM, uint32_t ICxEDG, uint32_t CCRX, uint32_t ARRX, uint32_t PSCX, timConfig_t* usrTim)
{
    // Assign the timer instance and mode to the user config structure
    usrTim->TIMX = TIMX;
    usrTim->MODE = MODE;
    
    // Assign other parameters to the user config structure
    usrTim->CH    = CH;       // Timer channel
    usrTim->ICxF  = ICxF;     // Input capture filter
    usrTim->OCxM  = OCxM;     // Output compare mode
    usrTim->ICxEDG = ICxEDG;  // Input capture edge detection
    usrTim->CCRX  = CCRX;     // Capture/compare register value
    usrTim->ARRX  = ARRX;     // Auto-reload register value
    usrTim->PSCX  = PSCX;     // Prescaler value

    // Call the timer initialization function with the config
    timer_init(usrTim);
}

/*******
FUNCTION: timer start
INPUTS: 
OUTPUTS: 
DESCRIPTION: starts timer
*******/
void timer_start(TIM_TypeDef* TIMX)
{
	TIMX->CNT = 0;
	TIMX->CR1 |= 1;
	
}

/*******
FUNCTION: timer stop
INPUTS: 
OUTPUTS: 
DESCRIPTION: stops timer
*******/
void timer_stop(TIM_TypeDef* TIMX)
{
	TIMX->CR1 &= ~(0x01);
}

/*******
FUNCTION: debug_print
INPUTS: 
OUTPUTS: 
DESCRIPTION: loop for debug print function (CURRENT VERSION ONLY SUPPORTS STRING INPUT WITHOUT FORMAT SPECIFIERS)
*******/
int debug_print(const char *str, ...)
{
    va_list lst;
    va_start(lst, str);
    while(*str!='\0')
    {
        stdout_putchar(*str);
		str++;
    }  
	va_end(lst);  
}

/*******
FUNCTION: RTC_init()
INPUTS: 
OUTPUTS: 
DESCRIPTION: loop for debug print function (CURRENT VERSION ONLY SUPPORTS STRING INPUT WITHOUT FORMAT SPECIFIERS)
*******/
void RTC_init(void)
{
	I2C1_init();
	gpio_usr_init(RTC_interrupt_PORT,RTC_interrupt,GPIO_MODER_GPI,GPIO_PUPDR_PU,0x00,&gpioStructConfig);
	interrupt_init(EXTI_PB,RTC_interrupt,EXTI_FE);
	//set alarms
	char test = 0;
	I2C1_byteWrite(RTC_ADDR,RTC_Control,0x1F);

	I2C1_byteWrite(RTC_ADDR,0x0D,0x80);
	I2C1_byteWrite(RTC_ADDR,0x0C,0x80);
	I2C1_byteWrite(RTC_ADDR,0x0B,0x80);
	I2C1_byteWrite(RTC_ADDR,RTC_Status,0x08);

	//set time
	#ifdef TIME_INIT
	I2C1_byteWrite(RTC_ADDR,RTC_Seconds,0x30);
	I2C1_byteWrite(RTC_ADDR,RTC_Minutes,0x47);
	I2C1_byteWrite(RTC_ADDR,RTC_Hours,0x19);
	#endif
	
	


}

/*EOF*/
