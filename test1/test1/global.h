/*
 * global.h
 *
 *  Created on: 21.02.2014
 *      Author: john
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_hibernate.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_i2c.h"
#include "inc/hw_ssi.h"
#include "inc/hw_ints.h"
#include "inc/hw_nvic.h"
#include "inc/hw_timer.h"

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/fpu.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/systick.h"
#include "utils/softi2c.h"
#include "utils/spi_flash.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"
#include "grlib/grlib.h"

#include "defines.h"

#include "menu/menu.h"
#include "hardware/buttons.h"
#include "hardware/ili9341/fonts/fonts.h"
#include "hardware/ili9341/ili9341.h"
#include "hardware/ili9341/pictures/pictures.h"
#include "hardware/interrupts/timer.h"
//#include "hardware/onewire/onewire.h"
//#include "hardware/onewire/ds18x20.h"
#include "hardware/onewire_new/onewire.h"
#include "hardware/onewire_new/ds18b20.h"
//#include "scheduler.h"
#include "hardware/i2c/ds1703.h"
#include "hardware/i2c/24x64.h"
#include "hardware/interrupts/interrupts.h"
#include "delay.h"


#define MAX_OW_DEVICES 4

extern tContext g_sContext;

static tSoftI2C g_sI2C;

#define BACKGROUND	ClrBlack
#define FOREGROUND	ClrWhite

#define sb(byte, bit) (byte |= (1 << (bit)))	//set bit
#define cb(byte, bit) (byte &= ~(1 << (bit)))	//clear bit
#define tb(byte, bit) (byte ^= (1 << (bit)))	//toggle bit
#define CheckBit(byte, bit) (byte & (1 << (bit)))

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define clockCyclesPerMicrosecond() ( 80000000L / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

static RectFillFg(const tRectangle *psRect, uint32_t foreGround)
{
	GrContextForegroundSet(&g_sContext, foreGround);
	GrRectFill(&g_sContext, psRect);
}

static RectDrawFg(const tRectangle *psRect, uint32_t foreGround)
{
	GrContextForegroundSet(&g_sContext, foreGround);
	GrRectDraw(&g_sContext, psRect);
}

inline static void delay_ms(unsigned long int millisecond)
{
	SysCtlDelay(((unsigned long) millisecond * (SysCtlClockGet() / (3 * 1000))));
}

inline static void delay_us(unsigned long int microsecond)
{
	SysCtlDelay(((unsigned long) microsecond * (SysCtlClockGet() / (3 * 1000000))));
}

////////////////////////////////////////////////////////////////////////////////////
// sensors variables
////////////////////////////////////////////////////////////////////////////////////
static uint32_t	InFuelImpulses;
static uint32_t	OutFuelImpulses;
static int32_t	TotalFuelImpulses;
static uint32_t	InDistanceImpulses;
////////////////////////////////////////////////////////////////////////////////////
// global variables
////////////////////////////////////////////////////////////////////////////////////

float CurrentDistance;
float TotalDistance;
float PeakConsumption;
float TotalConsumption;
float OverallConsumption;

#endif /* GLOBAL_H_ */
