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
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/fpu.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include "utils/softi2c.h"
#include "utils/spi_flash.h"
#include "utils/ustdlib.h"

#include "grlib/grlib.h"

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
#include "scheduler.h"
#include "hardware/i2c/ds1703.h"


#define MAX_OW_DEVICES 4

extern tContext g_sContext;

static tSoftI2C g_sI2C;

extern volatile uint32_t InFuelImpulses;
extern volatile uint32_t OutFuelImpulses;
extern volatile uint32_t InDistanceImpulses;

extern volatile float CurrentDistance;
extern volatile float TotalDistance;
extern volatile float PeakConsumption;
extern volatile float TotalConsumption;
extern volatile float OverallConsumption;

#define BACKGROUND	ClrBlack
#define FOREGROUND	ClrWhite

#define sb(byte, bit) (byte |= (1 << (bit)))	//set bit
#define cb(byte, bit) (byte &= ~(1 << (bit)))	//clear bit
#define tb(byte, bit) (byte ^= (1 << (bit)))	//toggle bit
#define CheckBit(byte, bit) (byte & (1 << (bit)))

#define F_CPU	80000000

//
//data
//
#define  TS_LCD_DATA_PORT     	GPIO_PORTH_BASE
#define  TS_LCD_D0   		    GPIO_PIN_0
#define  TS_LCD_D1   		    GPIO_PIN_1
#define  TS_LCD_D2   		    GPIO_PIN_2
#define  TS_LCD_D3    		    GPIO_PIN_3
#define  TS_LCD_D4   		    GPIO_PIN_4
#define  TS_LCD_D5   		    GPIO_PIN_5
#define  TS_LCD_D6	  	        GPIO_PIN_6
#define  TS_LCD_D7    		    GPIO_PIN_7
#define  TS_LCD_DATA            (GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0)
//	
//control
//
#define  TS_LCD_CONTROL_PORT   	GPIO_PORTD_BASE
#define  TS_LCD_CS     		    GPIO_PIN_0
#define  TS_LCD_RS    			GPIO_PIN_1
#define  TS_LCD_RD      		GPIO_PIN_2
#define  TS_LCD_WR     			GPIO_PIN_3
//#define  TS_LCD_RS      	    GPIO_PIN_4
#define  TS_LCD_BL              GPIO_PIN_5
#define  TS_LCD_RST             GPIO_PIN_6

#define TS_LCD_RS_SET()         		GPIOPinWrite(TS_LCD_CONTROL_PORT,(uint8_t)TS_LCD_RS,TS_LCD_RS);
#define TS_LCD_RS_CLR()               	GPIOPinWrite(TS_LCD_CONTROL_PORT,(uint8_t)TS_LCD_RS,0);
#define TS_LCD_RST_SET()              	GPIOPinWrite(TS_LCD_CONTROL_PORT,(uint8_t)TS_LCD_RST,TS_LCD_RST);
#define TS_LCD_RST_CLR()              	GPIOPinWrite(TS_LCD_CONTROL_PORT,(uint8_t)TS_LCD_RST,0);
#define TS_LCD_CS_SET()               	GPIOPinWrite(TS_LCD_CONTROL_PORT,(uint8_t)TS_LCD_CS,TS_LCD_CS);
#define TS_LCD_CS_CLR()               	GPIOPinWrite(TS_LCD_CONTROL_PORT,(uint8_t)TS_LCD_CS,0);
#define TS_LCD_RD_SET()               	GPIOPinWrite(TS_LCD_CONTROL_PORT,(uint8_t)TS_LCD_RD,TS_LCD_RD);
#define TS_LCD_RD_CLR()               	GPIOPinWrite(TS_LCD_CONTROL_PORT,(uint8_t)TS_LCD_RD,0);
#define TS_LCD_WR_SET()               	GPIOPinWrite(TS_LCD_CONTROL_PORT,(uint8_t)TS_LCD_WR,TS_LCD_WR);
#define TS_LCD_WR_CLR()               	GPIOPinWrite(TS_LCD_CONTROL_PORT,(uint8_t)TS_LCD_WR,0);
#define TS_LCD_BL_SET()               	GPIOPinWrite(TS_LCD_CONTROL_PORT,(uint8_t)TS_LCD_BL,TS_LCD_BL);
#define TS_LCD_BL_CLR()               	GPIOPinWrite(TS_LCD_CONTROL_PORT,(uint8_t)TS_LCD_BL,0);
#define TS_LCD_A11_SET()              	GPIOPinWrite(TS_LCD_CONTROL_PORT,(uint8_t)TS_LCD_A11,TS_LCD_A11);
#define TS_LCD_A11_CLR()              	GPIOPinWrite(TS_LCD_CONTROL_PORT,(uint8_t)TS_LCD_A11,0);
#define TS_LCD_DATA_PORT_OUT()        	GPIOPinTypeGPIOOutput(TS_LCD_DATA_PORT,(uint8_t)TS_LCD_DATA);
#define TS_LCD_DATA_PORT_IN()         	GPIOPinTypeGPIOInput(TS_LCD_DATA_PORT,(uint8_t)TS_LCD_DATA);
#define TS_LCD_DATA_PORT_ENABLE()     	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
#define TS_LCD_CONTROL_PORT_ENABLE()  	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
#define TS_LCD_CONTROL_PORT_OUT()     	GPIOPinTypeGPIOOutput(TS_LCD_CONTROL_PORT,0xff);

//typedef const struct time
//{
//	uint8_t year;
//	uint8_t month;
//	uint8_t day;
//	uint8_t hour;
//	uint8_t minute;
//	uint8_t second;
//} time_t;

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

#endif /* GLOBAL_H_ */
