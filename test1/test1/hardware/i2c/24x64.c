/*
 * 24x64.c
 *
 *  Created on: 20 мая 2014 г.
 *      Author: john
 */

#include "24x64.h"

void Init24x64(void)
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

    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);
    SysCtlPeripheralEnable(I2C1_BASE);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);

    I2CMasterEnable(I2C1_BASE);
}

uint8_t Read24x64()
{
     I2CMasterSlaveAddrSet( I2C1_MASTER_BASE, SLAVE_ADDRESS, false ) ;

     I2CMasterDataPut( I2C1_MASTER_BASE, 0x00 ) ;
     I2CMasterControl( I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START ) ;
     while(I2CMasterBusy(I2C1_MASTER_BASE))  {}

     I2CMasterDataPut( I2C1_MASTER_BASE, WRITE_ADDRESS ) ;
     I2CMasterControl( I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT ) ;
     while(I2CMasterBusy(I2C1_MASTER_BASE))  {}

     I2CMasterSlaveAddrSet( I2C1_MASTER_BASE, SLAVE_ADDRESS, true ) ;
     while(I2CMasterBusy(I2C1_MASTER_BASE))  {}

     I2CMasterControl( I2C1_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE ) ;
     while(I2CMasterBusy(I2C1_MASTER_BASE))  {}

     // Get Value
     return I2CMasterDataGet( I2C1_MASTER_BASE ) ;
}
void Write24x64()
{
     I2CMasterSlaveAddrSet( I2C1_MASTER_BASE, SLAVE_ADDRESS, false ) ;

     I2CMasterDataPut( I2C1_MASTER_BASE, 0x00 ) ;
     I2CMasterControl( I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START ) ;
     while(I2CMasterBusy(I2C1_MASTER_BASE))  {}

     I2CMasterDataPut( I2C1_MASTER_BASE, WRITE_ADDRESS ) ;
     I2CMasterControl( I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT ) ;
     while(I2CMasterBusy(I2C1_MASTER_BASE))  {}

     I2CMasterDataPut( I2C1_MASTER_BASE, DATA ) ;
     I2CMasterControl( I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH) ;
     while(I2CMasterBusy(I2C1_MASTER_BASE))  {}
}

