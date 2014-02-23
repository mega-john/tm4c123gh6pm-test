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

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/fpu.h"
#include "grlib/grlib.h"

#include "hardware/buttons.h"
#include "hardware/ili9341/fonts/fonts.h"
#include "hardware/ili9341/ili9341.h"
#include "menu/menu.h"
#include "hardware/interrupts/timer.h"

static tContext g_sContext;

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
