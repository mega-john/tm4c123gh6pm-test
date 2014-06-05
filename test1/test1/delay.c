/*
 * delay.c
 *
 *  Created on: 05 ���� 2014 �.
 *      Author: estarcev
 */

#include "delay.h"

static void (*SysTickCbFuncs[8])(uint32_t ui32TimeMS);
static unsigned long milliseconds = 0;

void timerInit()
{
    //
    //  Run at system clock at 80MHz
    //
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    //
    //  SysTick is used for delay() and delayMicroseconds()
    //
    SysTickPeriodSet(F_CPU / SYSTICKHZ);
    SysTickEnable();
    IntPrioritySet(FAULT_SYSTICK, SYSTICK_INT_PRIORITY);
    SysTickIntEnable();

    //
    //Initialize Timer5 to be used as time-tracker since beginning of time
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5); //not tied to launchpad pin
    TimerConfigure(TIMER5_BASE, TIMER_CFG_PERIODIC_UP);

    TimerLoadSet(TIMER5_BASE, TIMER_A, F_CPU / 1000);

    IntEnable(INT_TIMER5A);
    TimerIntEnable(TIMER5_BASE, TIMER_TIMA_TIMEOUT);

    TimerEnable(TIMER5_BASE, TIMER_A);

    IntMasterEnable();

}

unsigned long micros(void)
{
	return (milliseconds * 1000) + (HWREG(TIMER5_BASE + TIMER_O_TAV) / (F_CPU/1000000));
}

unsigned long millis(void)
{
	return milliseconds;
}

void delayMicroseconds(uint32_t microseconds)
{
	volatile uint32_t elapsedTime;
	uint32_t startTime = HWREG(NVIC_ST_CURRENT);
	do
	{
		elapsedTime = startTime-(HWREG(NVIC_ST_CURRENT) & 0x00FFFFFF);
	}while(elapsedTime <= microseconds * (F_CPU / 1000000));
}

void delayMilliseconds(uint32_t milliseconds)
{
	uint32_t i;
	for (i = 0; i < milliseconds; i++)
	{
		delayMicroseconds(1000);
	}
}

void Timer5IntHandlerA(void)
{

    TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
	milliseconds++;
}

void registerSysTickCb(void (*userFunc)(uint32_t))
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		if (!SysTickCbFuncs[i])
		{
			SysTickCbFuncs[i] = userFunc;
			break;
		}
	}
}

void SysTickIntHandler(void)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		if (SysTickCbFuncs[i])
		{
			SysTickCbFuncs[i](SYSTICKMS);
		}
	}
}
