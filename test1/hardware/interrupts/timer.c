/*
 * timer.c
 *
 *  Created on: 23.02.2014
 *      Author: john
 */
#include "timer.h"

void SetUpTimers()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);
	TimerPrescaleSet(TIMER0_BASE, TIMER_A, 8000);

    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / 1000);

    //
    // Enable processor interrupts.
    //
    IntMasterEnable();

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
