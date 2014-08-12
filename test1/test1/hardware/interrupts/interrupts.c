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
		UARTprintf("\nInFuelInterrupt");
		GPIOIntClear(IN_FUEL_PORT, IN_FUEL_PIN);  // Clear interrupt flag
	}
}

void OutFuelInterrupt(void)
{
	if (GPIOIntStatus(OUT_FUEL_PORT, false) & OUT_FUEL_PIN)
	{
		OutFuelImpulses++;
		UARTprintf("\nOutFuelInterrupt");
		GPIOIntClear(OUT_FUEL_PORT, IN_FUEL_PIN);  // Clear interrupt flag
	}
}

void DistanceImpulseInterrupt(void)
{
	if (GPIOIntStatus(IN_DISTANCE_PORT, false) & IN_DISTANCE_PIN)
	{
		InDistanceImpulses++
		UARTprintf("\nDistanceImpulseInterrupt");
		GPIOIntClear(IN_DISTANCE_PORT, IN_FUEL_PIN);  // Clear interrupt flag
	}
}

void ShutdownInterrupt(void)
{
	if (GPIOIntStatus(SHUTDOWN_PORT, false) & SHUTDOWN_PIN)
	{
		UARTprintf("\nShutdownInterrupt");
		GPIOIntClear(SHUTDOWN_PORT, SHUTDOWN_PIN);  // Clear interrupt flag
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
	SetupInterrupt(SHUTDOWN_PERIPH, SHUTDOWN_PORT, SHUTDOWN_PIN, true, ShutdownInterrupt);
}
