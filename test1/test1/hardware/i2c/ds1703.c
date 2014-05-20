/*
 * ds1703.c
 *
 *  Created on: 20 мая 2014 г.
 *      Author: john
 */

#include "ds1703.h"

void InitDS1703(void)
{
    //
    // Enable Peripheral Clocks
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable pin PA7 for I2C1 I2C1SDA
    //
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

    //
    // Enable pin PA6 for I2C1 I2C1SCL
    //
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
}
