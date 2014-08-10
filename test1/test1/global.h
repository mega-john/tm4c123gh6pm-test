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
#include <float.h>

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
#include "driverlib/onewire.h"
#include "driverlib/eeprom.h"
#include "driverlib/flash.h"
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
#include "hardware/onewire/onewire.h"
#include "hardware/onewire/ds18x20.h"
//#include "hardware/onewire_new/onewire.h"
//#include "hardware/onewire_new/ds18b20.h"
//#include "scheduler.h"
#include "hardware/i2c/ds1703.h"
#include "hardware/i2c/24x64.h"
#include "hardware/interrupts/interrupts.h"
#include "delay.h"

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
float Distance;//���������� �� ������� ������� � ������
float TotalDistance;//����� �������� � ������

float AverageConsumptionInTravel;//������� ������ ������� �� ������� �/100��
float CurrentConsumptionInTravel;//������� ������ ������� �� ������� �/100��
float AverageConsumption;//����� ������� ������ ������� � �/100��
float FuelSpetInTravel;//������������� ������� �� ������� � �
float FuelTotalSpent;//����� ������������� ������� � �


#endif /* GLOBAL_H_ */
