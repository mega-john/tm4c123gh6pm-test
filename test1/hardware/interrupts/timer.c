/*
 * timer.c
 *
 *  Created on: 23.02.2014
 *      Author: john
 */
#include "timer.h"


#define SW1 			GPIO_PIN_4
#define SW2 			GPIO_PIN_0
#define LED_RED 		GPIO_PIN_1
#define LED_BLUE 	GPIO_PIN_2
#define LED_GREEN 	GPIO_PIN_3
#define ALL_LEDS		(LED_RED | LED_BLUE | LED_GREEN)

//extern uint8_t red_state;//, green_state, blue_state;
uint32_t intCount = 0;

void SetUpTimers()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

	IntMasterEnable();

	TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
//	TimerPrescaleSet(TIMER0_BASE, TIMER_A, 2);

	uint64_t tValue = SysCtlClockGet()/2000;
    TimerLoadSet (TIMER0_BASE, TIMER_A, tValue);
//    TimerLoadSet64 (TIMER0_BASE, tValue);

    //
    // Configure the Timer0B interrupt for timer timeout.
    //
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Enable the Timer0B interrupt on the processor (NVIC).
    //
    IntEnable(INT_TIMER0A);

    //
    // Enable Timer0B.
    //
    TimerEnable(TIMER0_BASE, TIMER_A);

}


void Timer0IntHandlerA()
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
//	if (++intCount >= 1000)
	{
		if (red_state == 0)
		{
			red_state = GPIO_PIN_1;
		}
		else
		{
			red_state = 0;
		}
		intCount = 0;
	}
//	GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_BLUE | LED_GREEN, red_state | blue_state | green_state);
//    GrStringDraw(&g_sContext, red_state ? "a  " : "  b", -1, 10, 290, 0);
}
void Timer0IntHandlerB()
{
}
void Timer1IntHandlerA()
{
}
void Timer1IntHandlerB()
{
}
void Timer2IntHandlerA()
{
}
void Timer2IntHandlerB()
{
}
void Timer3IntHandlerA()
{
}
void Timer3IntHandlerB()
{
}
void Timer4IntHandlerA()
{
}
void Timer4IntHandlerB()
{
}
void Timer5IntHandlerA()
{
}
void Timer5IntHandlerB()
{
}
