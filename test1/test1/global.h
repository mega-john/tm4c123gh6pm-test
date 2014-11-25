/*
 * global.h
 *
 *  Created on: 21.02.2014
 *      Author: john
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

//#define PART_TM4C123GH6PM

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <float.h>
#include <math.h>

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
//#include "inc/hw_onewire.h"

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
#include "driverlib/onewire.h"
#include "driverlib/eeprom.h"
#include "driverlib/flash.h"
#include "driverlib/onewire.h"
#include "utils/softi2c.h"
#include "utils/spi_flash.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"
#include "grlib/grlib.h"

#include "defines.h"

#include "menu/menu.h"
#include "hardware/buttons.h"
#include "hardware/Display/display.h"

#include "hardware/interrupts/timer.h"
#include "hardware/onewire/onewire.h"
#include "hardware/onewire/ds18x20.h"
#include "hardware/i2c/ds1703.h"
#include "hardware/i2c/24x64.h"
#include "hardware/interrupts/interrupts.h"
#include "hardware/EEPROM/internalEEPROM.h"
#include "delay.h"
#include "lib/ftoa.h"

#define F_CPU   80000000L

#define MAX_OW_DEVICES 4

extern tContext g_sContext;

static tSoftI2C g_sI2C;

#define BACKGROUND	ClrBlack
#define FOREGROUND	ClrWhite

#define sb(byte, bit) (byte |= (1 << (bit)))	//set bit
#define cb(byte, bit) (byte &= ~(1 << (bit)))	//clear bit
#define tb(byte, bit) (byte ^= (1 << (bit)))	//toggle bit
#define CheckBit(byte, bit) (byte & (1 << (bit)))

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define constrain(amt, low, high) ((amt) <= (low) ? (low) : ((amt) >= (high) ? (high) : (amt)))
#define round(x) ((x) >= 0 ? (long)((x) + 0.5) : (long)((x) - 0.5))
#define radians(deg) ((deg) * DEG_TO_RAD)
#define degrees(rad) ((rad) * RAD_TO_DEG)
#define sq(x) ((x) * (x))

#define clockCyclesPerMicrosecond() (80000000L / 1000000L)
#define clockCyclesToMicroseconds(a) ((a) / clockCyclesPerMicrosecond())
#define microsecondsToClockCycles(a) ((a) * clockCyclesPerMicrosecond())

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define FAST_GPIOPinWrite(ulPort, ucPins, ucVal) (HWREG(ulPort + (GPIO_O_DATA + (ucPins << 2))) = ucVal)

typedef struct
{
    bool update_temperature;
    bool update_menu;
} update_flags;

typedef union
{
    struct
    {
        uint8_t seconds;
        uint8_t minutes;
        uint8_t hours;
        uint8_t days;
    }dt;
    uint16_t tt;
}TravelTime;

typedef struct
{
    TravelTime tt;

    float distance_in_meters;
//    uint8_t seconds;
//    uint8_t seconds;
}ReportItem;

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
float Distance;//рассто€ние за текущую поездку в метрах
float TotalDistance;//всего пройдено в метрах

float AverageConsumptionInTravel;//средний расход топлива за поездку л/100км
float CurrentConsumptionInTravel;//текущий расход топлива за поездку л/100км
float AverageConsumption;//общий средний расход топлива в л/100км
float FuelSpetInTravel;//израсходовано топлива за поездку в л
float FuelTotalSpent;//всего израсходовано топлива в л


#endif /* GLOBAL_H_ */
