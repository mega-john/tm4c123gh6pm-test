/*
 * 24x64.c
 *
 *  Created on: 20 мая 2014 г.
 *      Author: john
 */

#include "24x64.h"

void InitI2C(void)
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

uint32_t I2C_Write(uint8_t dev_addr, uint16_t addr, uint8_t *buff, uint32_t len)
{
    uint32_t l = len;

    I2CMasterSlaveAddrSet(I2C0_BASE, dev_addr, false);
    I2CMasterDataPut(I2C0_BASE, (unsigned char) (addr >> 8));
    I2CMasterControl(I2C0_BASE, I2C_MCS_START | I2C_MCS_RUN);
    while (I2CMasterBusy(I2C0_BASE))
    {
    };

    I2CMasterDataPut(I2C0_BASE, (unsigned char) addr);
    I2CMasterControl(I2C0_BASE, I2C_MCS_RUN);
    while (I2CMasterBusy(I2C0_BASE))
    {
    };

    while (l--)
    {
        I2CMasterDataPut(I2C0_BASE, *buff++);
        if (l == 0)
        {
            I2CMasterControl(I2C0_BASE, I2C_MCS_STOP | I2C_MCS_RUN);
        }
        else
        {
            I2CMasterControl(I2C0_BASE, I2C_MCS_RUN);
        }
        while (I2CMasterBusy(I2C0_BASE))
        {
        };
    }

    return len;
}

uint32_t I2C_Read(uint8_t dev_addr, uint16_t addr, uint8_t *buff, uint32_t len)
{
    uint32_t l;

    if (len == 0)
    {
        return 0;
    }

    if (!buff)
    {
        return 0;
    }

    I2CMasterSlaveAddrSet(I2C0_BASE, dev_addr, false);
    I2CMasterDataPut(I2C0_BASE, (unsigned char) (addr >> 8));
    I2CMasterControl(I2C0_BASE, I2C_MCS_START | I2C_MCS_RUN);
    while (I2CMasterBusy(I2C0_BASE))
    {
    };

    I2CMasterDataPut(I2C0_BASE, (unsigned char) addr);
    I2CMasterControl(I2C0_BASE, I2C_MCS_RUN);
    while (I2CMasterBusy(I2C0_BASE))
    {
    };

    if (len == 1)
    {
        I2CMasterSlaveAddrSet(I2C0_BASE, dev_addr, true);
        I2CMasterControl(I2C0_BASE, I2C_MCS_START | I2C_MCS_STOP | I2C_MCS_RUN);
        while (I2CMasterBusy(I2C0_BASE))
        {
        };
        *buff = I2CMasterDataGet(I2C0_BASE);
    }
    else
    {
        l = len;
        I2CMasterSlaveAddrSet(I2C0_BASE, dev_addr, true);
        I2CMasterControl(I2C0_BASE, I2C_MCS_ACK | I2C_MCS_START | I2C_MCS_RUN);
        while (I2CMasterBusy(I2C0_BASE))
        {
        };
        *buff++ = I2CMasterDataGet(I2C0_BASE);

        while (l--)
        {
            if (l == 0)
            {
                I2CMasterControl(I2C0_BASE, I2C_MCS_STOP | I2C_MCS_RUN);
            }
            else
            {
                I2CMasterControl(I2C0_BASE, I2C_MCS_ACK | I2C_MCS_RUN);
            }
            while (I2CMasterBusy(I2C0_BASE))
            {
            };
            *buff++ = I2CMasterDataGet(I2C0_BASE);
        }
    }

    return len;
}

uint32_t Read24x64(uint16_t addr, uint8_t *buff, uint32_t len)
{
    return I2C_Read(DEV_ADDR_24CXX, addr, buff, len);
//    I2CMasterSlaveAddrSet(I2C1_BASE, DEV_ADDR_24CXX, false);
//
//    I2CMasterDataPut(I2C1_BASE, 0x00);
//    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
//    while(I2CMasterBusy(I2C1_BASE))
//    {
//    }
//
//    I2CMasterDataPut(I2C1_BASE, address);
//    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
//    while(I2CMasterBusy(I2C1_BASE))
//    {
//    }
//
//    I2CMasterSlaveAddrSet(I2C1_BASE, DEV_ADDR_24CXX, true);
//    while(I2CMasterBusy(I2C1_BASE))
//    {
//    }
//
//    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
//    while(I2CMasterBusy(I2C1_BASE))
//    {
//    }
//
//    // Get Value
//    return I2CMasterDataGet( I2C1_BASE);
}
uint32_t Write24x64(uint16_t addr, uint8_t *buff, uint32_t len)
{
    return I2C_Read(DEV_ADDR_24CXX, addr, buff, len);

//    I2CMasterSlaveAddrSet(I2C1_BASE, DEV_ADDR_24CXX, false);
//
//    I2CMasterDataPut(I2C1_BASE, 0x00);
//    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
//    while(I2CMasterBusy(I2C1_BASE))
//    {
//    }
//
//    I2CMasterDataPut(I2C1_BASE, address);
//    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
//    while(I2CMasterBusy(I2C1_BASE))
//    {
//    }
//
//    I2CMasterDataPut(I2C1_BASE, data);
//    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
//    while(I2CMasterBusy(I2C1_BASE))
//    {
//    }
}
