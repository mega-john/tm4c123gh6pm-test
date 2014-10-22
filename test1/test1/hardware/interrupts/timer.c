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
#define LED_BLUE 		GPIO_PIN_2
#define LED_GREEN 		GPIO_PIN_3
#define ALL_LEDS		(LED_RED | LED_BLUE | LED_GREEN)
#define LED_RED_ON		LED_RED
#define LED_RED_OFF		0

#define CPU_CLOCK           80000000UL
#define TIMER_PRESCALLER    127UL
#define MS(x)               (x / 1000.0)
#define US(x)               (x / 1000000.0)
#define NS(x)               (x / 1000000000.0)

#define SETUP_TIMER_MS(x)  ((CPU_CLOCK / (TIMER_PRESCALLER + 1UL)) * MS(x));
#define SETUP_TIMER_US(x)  ((CPU_CLOCK / (TIMER_PRESCALLER + 1UL)) * US(x));
#define SETUP_TIMER_NS(x)  ((CPU_CLOCK / (TIMER_PRESCALLER + 1UL)) * NS(x));


//extern uint8_t red_state;//, green_state, blue_state;
uint32_t intCount = 0;

bool b = false;

void SetupTimer0()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
//    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOB);
//    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
//    GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_DIR_MODE_OUT);
//    GPIOPinTypeGPIOOutput(GPIO_PORTB_AHB_BASE, GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerDisable(TIMER0_BASE, TIMER_BOTH);
	TimerConfigure(TIMER0_BASE,	TIMER_CFG_PERIODIC_UP | TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_B_PERIODIC);
	//80mHz/128 = 625000Hz, тоесть 62500 тиков ~ 100ms
	TimerPrescaleSet(TIMER0_BASE, TIMER_A, TIMER_PRESCALLER);
	TimerPrescaleSet(TIMER0_BASE, TIMER_B, TIMER_PRESCALLER);
	//	uint64_t tValue = SysCtlClockGet()/2000;
    uint32_t t = SETUP_TIMER_NS(10000);
    uint32_t t1 = SETUP_TIMER_MS(10);
    uint32_t t2 = SETUP_TIMER_MS(100);
    uint32_t t3 = SETUP_TIMER_MS(1000);
    uint32_t t4 = SETUP_TIMER_MS(10000);
//    TimerLoadSet(TIMER0_BASE, TIMER_A, t4);
    TimerLoadSet(TIMER0_BASE, TIMER_B, t);
	//
	// Configure the Timer0 interrupt for timer timeout.
	//
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
	//
	// Enable the Timer0 interrupt on the processor (NVIC).
	//
    IntEnable(INT_TIMER0A);
    IntEnable(INT_TIMER0B);
	//
	// Enable Timer0.
	//
	TimerEnable(TIMER0_BASE, TIMER_BOTH);
}

void SetupTimer1()
{

}

void SetupTimer2()
{

}

void SetupTimer3()
{

}

void SetupTimer4()
{
    //
    // Enable the peripherals used by this example.
    //
    // TODO: Update this depending upon the general purpose timer and
    // CCP pin you intend using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    //
    // Configure PM0 as the CCP0 pin for timer 4.
    //
    // TODO: This is set up to use GPIO PM0 which can be configured
    // as the CCP0 pin for Timer4 and also happens to be attached to
    // a switch on the EK-LM4F232 board.  Change this configuration to
    // correspond to the correct pin for your application.
    //
    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_0);
    GPIOPinConfigure(GPIO_PC0_T4CCP0);
    //
    // Set the pin to use the internal pull-up.
    //
    // TODO: Remove or replace this call to correspond to the wiring
    // of the CCP pin you are using.  If your board has an external
    // pull-up or pull-down, this will not be required.
    //
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Configure the timers in downward edge count mode.
    //
    // TODO: Modify this to configure the specific general purpose
    // timer you are using.  The timer choice is intimately tied to
    // the pin whose edges you want to capture because specific CCP
    // pins are connected to specific timers.
    //
    TimerConfigure(TIMER4_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_COUNT));
    TimerControlEvent(TIMER4_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
    TimerLoadSet(TIMER4_BASE, TIMER_A, 9);
    TimerMatchSet(TIMER4_BASE, TIMER_A, 0);
    //
    // Setup the interrupt for the edge capture timer.  Note that we
    // use the capture match interrupt and NOT the timeout interrupt!
    //
    // TODO: Modify to enable the specific timer you are using.
    //
    IntEnable(INT_TIMER4A);
    TimerIntEnable(TIMER4_BASE, TIMER_CAPA_MATCH);
    //
    // Enable the timer.
    //
    // TODO: Modify to enable the specific timer you are using.
    //
    TimerEnable(TIMER4_BASE, TIMER_A);
}

void SetupTimer5()
{

}

void SetUpTimers()
{
	SetupTimer0();
}

void Timer0IntHandlerA()
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	if (++intCount >= 10)
	{
		if (red_state == 0)
		{
			red_state = LED_RED_ON;
//	        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0);
		}
		else
		{
			red_state = LED_RED_OFF;
//	        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_PIN_5);
		}
		intCount = 0;
	}
//	GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_BLUE | LED_GREEN, red_state | blue_state | green_state);
//    GrStringDraw(&g_sContext, red_state ? "a  " : "  b", -1, 10, 290, 0);
}
void Timer0IntHandlerB()
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
//    TimerLoadSet(TIMER0_BASE, TIMER_B, 650);
    if(b)
    {
//        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0);
        HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (GPIO_PIN_5 << 2))) = 0;
        b = false;
    }
    else
    {
//        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_PIN_5);
        HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (GPIO_PIN_5 << 2))) = GPIO_PIN_5;
        b = true;
    }
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
//void Timer5IntHandlerA()//used in delay.c
//{
//}
void Timer5IntHandlerB()
{
}
