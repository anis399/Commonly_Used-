/*******************************************************************************
 * File Timer.h
 *	
 *  Created on: Oct 8, 2022
 *  Author: Anis Shakkour
 *  Email:  anis.shakkour399@gmail.com
 * 
 ******************************************************************************/

/******************************************************************************
 * Description  : Links used
 *					1. https://docs.silabs.com/gecko-platform/4.1/emlib/api/efr32xg22/group-timer 
 *					2. https://www.silabs.com/documents/public/reference-manuals/efr32xg22-rm.pdf
 * Usage        :
 * Known Errors :
 * ToDo         :
 *****************************************************************************/

/******************************************************************************
 * Multiple include protection
 *****************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

/******************************************************************************
 * Includes
 *****************************************************************************/
 //SiLabs Peripheral
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_timer.h"

//Instances

//Dev Peripherals
#include "app.h"


/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Defines
 *****************************************************************************/
#define DESIRED_TIMER         TIMER0
#define DESIRED_TIMER_CLOCK   cmuClock_TIMER0
#define DESIRED_TIMER_IRQ     TIMER0_IRQn
#define DESIRED_TIMER_FREQ    10 //Hz

#define DESIRED_PWM_TIMER         TIMER1
#define DESIRED_PWM_TIMER_CLOCK   cmuClock_TIMER1
#define DESIRED_PWM_TIMER_IRQ     TIMER1_IRQn
#define DESIRED_PWM_FREQ          1000 //Hz
#define DESIRED_DUTY_CYCLE        30 //%
#define DESIRED_PWM_PIN			  4
#define DESIRED_PWM_PORT		  gpioPortA

/******************************************************************************
 * Typedef & Enums
 *****************************************************************************/
typedef enum {
	funcFailded,
	funcSuccess,
	funcOnGoing,
}RetVal_t;

/******************************************************************************
 * Interface Functions
 *****************************************************************************/
RetVal_t Timer_P_TimeInit(void);
RetVal_t Timer_P_PWMInit(void);

/******************************************************************************
 * END
 *****************************************************************************/
#endif /* TIMER_H_ */
