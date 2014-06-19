/*
 * interrupts.c
 *
 *  Created on: 23.02.2014
 *      Author: john
 */
#include "interrupts.h"

void InFuelInterrupt(void)
{
	if (GPIOIntStatus(IN_FUEL_PORT, false) & IN_FUEL_PIN)
	{
		InFuelImpulses++;
		// PF4 was interrupt cause
		UARTprintf("\nInFuelInterrupt");
//		GPIOIntRegister(GPIO_PORTF_BASE, onButtonUp); // Register our handler function for port F
//		GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_RISING_EDGE); // Configure PF4 for rising edge trigger
		GPIOIntClear(IN_FUEL_PORT, IN_FUEL_PIN);  // Clear interrupt flag
	}
}

void OutFuelInterrupt(void)
{
	if (GPIOIntStatus(OUT_FUEL_PORT, false) & OUT_FUEL_PIN)
	{
		// PF4 was interrupt cause
		UARTprintf("\nOutFuelInterrupt");
//		GPIOIntRegister(GPIO_PORTF_BASE, onButtonUp); // Register our handler function for port F
//		GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_RISING_EDGE); // Configure PF4 for rising edge trigger
		GPIOIntClear(OUT_FUEL_PORT, IN_FUEL_PIN);  // Clear interrupt flag
	}
}

void DistanceImpulseInterrupt(void)
{
	if (GPIOIntStatus(IN_DISTANCE_PORT, false) & IN_DISTANCE_PIN)
	{
		// PF4 was interrupt cause
		UARTprintf("\nDistanceImpulseInterrupt");
//		GPIOIntRegister(GPIO_PORTF_BASE, onButtonUp); // Register our handler function for port F
//		GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_RISING_EDGE); // Configure PF4 for rising edge trigger
		GPIOIntClear(IN_DISTANCE_PORT, IN_FUEL_PIN);  // Clear interrupt flag
	}
}

void SetupInterrupt(uint32_t Peripheral, uint32_t Port, uint8_t Pin, bool isFalling, void (*IntHandler)(void))
{
	// Pin F4 setup
	SysCtlPeripheralEnable(Peripheral); // Enable port F
	GPIOPinTypeGPIOInput(Port, Pin);
	GPIOPadConfigSet(Port, Pin, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	// Interrupt setup
	GPIOIntDisable(Port, Pin);
	GPIOIntClear(Port, Pin);
	GPIOIntRegister(Port, IntHandler); // Register our handler function for port F
	GPIOIntTypeSet(Port, Pin, isFalling ? GPIO_FALLING_EDGE : GPIO_RISING_EDGE);
	GPIOIntEnable(Port, Pin);
}

void SetupExternalInterrupts(void)
{
	SetupInterrupt(IN_FUEL_PERIPH, IN_FUEL_PORT, IN_FUEL_PIN, true, InFuelInterrupt);
	SetupInterrupt(OUT_FUEL_PERIPH, OUT_FUEL_PORT, OUT_FUEL_PIN, true, OutFuelInterrupt);
	SetupInterrupt(IN_DISTANCE_PERIPH, IN_DISTANCE_PORT, IN_DISTANCE_PIN, true, DistanceImpulseInterrupt);
}




