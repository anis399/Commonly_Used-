/*******************************************************************************
 * File Timer.c
 *
 *  Created on: Oct 23, 2022
 *  Author: Anis Shakkour
 *  Email:  anis.shakkour399@gmail.com
 ******************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Timer.h"

/******************************************************************************
 * Data types
 *****************************************************************************/
// Duty cycle global variable for IRQ handler use
static volatile float dutyCycle;

/******************************************************************************
 * Extern
 *****************************************************************************/

/******************************************************************************
 * Private Function Prototypes
 *****************************************************************************/
/*******************************************************************************
 * Function name:
 *
 * Description  : Init timer selected from the header file
 * Parameters   :
 * Returns      : funcSuccess if timer init is successful
 *
 * Known issues : resolution was not properly tested
 * Note         : Please make sure to set the desired timer from the Timer.h header file
 *                make sure to also select the relevant DESIRED_TIMER_IRQ and DESIRED_TIMER_IRQ
 ******************************************************************************/
RetVal_t Timer_P_TimeInit(void)
{
  uint32_t timerFreq, topValue;
  TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;

  //Enable clock for timer module
  CMU_ClockEnable(DESIRED_TIMER_CLOCK, true);

  // Don't start counter on initialization
  timerInit.enable = false;
  timerInit.debugRun = true;

  TIMER_Init(DESIRED_TIMER, &timerInit);

  // Set top value to overflow desired frequency
  timerFreq = CMU_ClockFreqGet(DESIRED_TIMER_CLOCK) / (timerInit.prescale + 1);
  topValue = (timerFreq / DESIRED_TIMER_FREQ);
  TIMER_TopSet(DESIRED_TIMER, topValue);

  // Now start the TIMER
  TIMER_Enable(DESIRED_TIMER, true);

  // Enable TIMER and get an interrupt on overflow
  TIMER_IntEnable(DESIRED_TIMER, TIMER_IEN_OF);
  NVIC_EnableIRQ(DESIRED_TIMER_IRQ);

  return funcSuccess;
}

/*******************************************************************************
 * Function name: Timer_P_PWMInit
 *
 * Description  : Creates a PWM signal and routed to PortA04
 * Parameters   :
 * Returns      :
 *
 * Known issues :
 * Note         :
 ******************************************************************************/
RetVal_t Timer_P_PWMInit(void)
{
  uint32_t timerFreq, topValue, dutyCount;
  TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
  TIMER_InitCC_TypeDef timerCCInit = TIMER_INITCC_DEFAULT;

  //Enable clock for timer module
  CMU_ClockEnable(DESIRED_PWM_TIMER_CLOCK, true);

  // Don't start counter on initialization
  timerInit.enable = false;

  // PWM mode sets/clears the output on compare/overflow events
  timerCCInit.mode = timerCCModePWM;

  TIMER_Init(DESIRED_PWM_TIMER, &timerInit);

  // Route CC0 output to PA6
  GPIO->TIMERROUTE[0].ROUTEEN  = GPIO_TIMER_ROUTEEN_CC0PEN;
  GPIO->TIMERROUTE[0].CC0ROUTE = (DESIRED_PWM_PORT << _GPIO_TIMER_CC0ROUTE_PORT_SHIFT)
                    | (DESIRED_PWM_PIN << _GPIO_TIMER_CC0ROUTE_PIN_SHIFT);

  TIMER_InitCC(DESIRED_PWM_TIMER, 0, &timerCCInit);

  // Set top value to overflow at the desired PWM_FREQ frequency
  timerFreq = CMU_ClockFreqGet(DESIRED_PWM_TIMER_CLOCK) / (timerInit.prescale + 1);
  topValue = (timerFreq / DESIRED_PWM_FREQ);
  TIMER_TopSet(DESIRED_PWM_TIMER, topValue);

  // Set dutyCycle global variable and compare value for initial duty cycle
  dutyCycle = DESIRED_DUTY_CYCLE;
  dutyCount = (topValue * DESIRED_DUTY_CYCLE) / 100;
  TIMER_CompareSet(DESIRED_PWM_TIMER, 0, dutyCount);

  // Now start the TIMER
  TIMER_Enable(DESIRED_PWM_TIMER, true);

  // Enable TIMER0 compare event interrupts to update the duty cycle
  TIMER_IntEnable(DESIRED_PWM_TIMER, TIMER_IEN_CC0);
  NVIC_EnableIRQ(DESIRED_PWM_TIMER_IRQ);

  return funcSuccess;
}

/*******************************************************************************
 * Function name: TIMER1_IRQHandler
 *
 * Description  : Interrupt handler for TIMER used in PWM mode, the duty cycle
 *                of the output waveform does not change unless the value of
 *                the dutyCycle global variable is modified outside the scope of
 *                this function.
 * Parameters   :
 * Returns      :
 *
 * Known issues :
 * Note         :
 ******************************************************************************/
void TIMER1_IRQHandler(void)
{
  uint32_t newDutyCount;

  // Acknowledge the interrupt
  uint32_t flags = TIMER_IntGet(DESIRED_PWM_TIMER);
  TIMER_IntClear(DESIRED_PWM_TIMER, flags);

  // Calculate new duty cycle count
  newDutyCount = (uint32_t)((TIMER_TopGet(DESIRED_PWM_TIMER) * dutyCycle) / 100);

  // Write OCB to update the duty cycle of the next waveform period
  TIMER_CompareBufSet(DESIRED_PWM_TIMER, 0, newDutyCount);
}


/*******************************************************************************
 * Function name: TIMER0_IRQHandler
 *
 * Description  : Interrupt handler for TIMER used in up count.
 *                this function will also clear timer flag to enable to
 *                restart counter
 * Parameters   :
 * Returns      :
 *
 * Known issues :
 * Note         :
 ******************************************************************************/
void TIMER0_IRQHandler(void)
{
  // Acknowledge the interrupt
  uint32_t flags = TIMER_IntGet(TIMER0);
  TIMER_IntClear(TIMER0, flags);

  //Perhaps a flag should be raised
  /**
   *  Dev Code
   *
   */
}
