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

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/fpu.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include "utils/softi2c.h"
#include "utils/spi_flash.h"

#include "grlib/grlib.h"

#include "hardware/buttons.h"
#include "hardware/ili9341/fonts/fonts.h"
#include "hardware/ili9341/ili9341.h"
#include "menu/menu.h"
#include "hardware/interrupts/timer.h"

extern tContext g_sContext;

static tSoftI2C g_sI2C;

static volatile uint32_t InFuelImpulses;
static volatile uint32_t OutFuelImpulses;
static volatile uint32_t InDistanceImpulses;

static volatile float CurrentDistance;
static volatile float TotalDistance;
static volatile float PeakConsumption;
static volatile float TotalConsumption;
static volatile float OverallConsumption;

#define BACKGROUND	ClrBlack
#define FOREGROUND	ClrWhite

typedef const struct time
{
	uint8_t	year;
	uint8_t	month;
	uint8_t	day;
	uint8_t	hour;
	uint8_t	minute;
	uint8_t	second;
}time_t;

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


#endif /* GLOBAL_H_ */
