/*
 * 24x64.c
 *
 *  Created on: 20 ��� 2014 �.
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

    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), true);

    I2CMasterEnable(I2C1_BASE);
}



//uint32_t I2C_Write(uint8_t dev_addr, uint16_t addr, uint8_t *buff, uint32_t len)
//{
//    uint32_t l = len;
//
//    I2CMasterSlaveAddrSet(I2C1_BASE, (dev_addr >> 1), false);// adr (0xA0) 1010 translated to 0x50.  Note R/W bit = 0
//
//    I2CMasterDataPut(I2C1_BASE, (uint8_t)(addr >> 8));// 1st Byte (MSB of eeprom adr)
//    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);// generates the I2C START Condition
//    while(I2CMasterBusy(I2C1_BASE));
//
//    I2CMasterDataPut(I2C1_BASE, (uint8_t)(addr & 0x00FF));// 2nd Byte (LSB of eeprom adr)
//    I2CMasterControl(I2C1_BASE,  I2C_MASTER_CMD_BURST_SEND_CONT);// NO START nor STOP Condx.
//    while(I2CMasterBusy(I2C1_BASE));
//
//    while (l--)
//    {
//        I2CMasterDataPut(I2C1_BASE, *buff++);
//        if (l == 0)
//        {
//            I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
//        }
//        else
//        {
//            I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
//        }
//        while (I2CMasterBusy(I2C1_BASE));
//    }
//
//    return len;
//}
//
//uint32_t I2C_Read(uint8_t dev_addr, uint16_t addr, uint8_t *buff, uint32_t len)
//{
//    uint32_t l;
//
//    if (len == 0)
//    {
//        return 0;
//    }
//
//    if (!buff)
//    {
//        return 0;
//    }
//
//    I2CMasterSlaveAddrSet(I2C1_BASE, (dev_addr >> 1), false);// adr (0xA0) 1010 translated to 0x50.  Note R/W bit = 0
//    I2CMasterDataPut(I2C1_BASE, (uint8_t) (addr >> 8));// 1st Byte (MSB of eeprom adr)
//    I2CMasterControl(I2C1_BASE, I2C_MCS_START | I2C_MCS_RUN);
//    while (I2CMasterBusy(I2C1_BASE))
//    {
//    };
//
//    I2CMasterDataPut(I2C1_BASE, (uint8_t) (addr & 0x00FF));// 2nd Byte (LSB of eeprom adr)
//    I2CMasterControl(I2C1_BASE, I2C_MCS_RUN);
//    while (I2CMasterBusy(I2C1_BASE))
//    {
//    };
//
//    if (len == 1)
//    {
//        I2CMasterSlaveAddrSet(I2C1_BASE, (dev_addr >> 1), true);
//        I2CMasterControl(I2C1_BASE, I2C_MCS_START | I2C_MCS_STOP | I2C_MCS_RUN);
//        while (I2CMasterBusy(I2C1_BASE))
//        {
//        };
//        *buff = I2CMasterDataGet(I2C1_BASE);
//    }
//    else
//    {
//        l = len;
//        I2CMasterSlaveAddrSet(I2C1_BASE, (dev_addr >> 1), true);
//        I2CMasterControl(I2C1_BASE, I2C_MCS_ACK | I2C_MCS_START | I2C_MCS_RUN);
//        while (I2CMasterBusy(I2C1_BASE))
//        {
//        };
//        uint8_t d = I2CMasterDataGet(I2C1_BASE);
//        *buff++ = d;
//
//        while (l--)
//        {
//            if (l == 0)
//            {
//                I2CMasterControl(I2C1_BASE, I2C_MCS_STOP | I2C_MCS_RUN);
//            }
//            else
//            {
//                I2CMasterControl(I2C1_BASE, I2C_MCS_ACK | I2C_MCS_RUN);
//            }
//            while (I2CMasterBusy(I2C1_BASE))
//            {
//            };
//            d = I2CMasterDataGet(I2C1_BASE);
//            *buff++ = d;
//        }
//    }
//
//    return len;
//}

uint32_t I2C_Read(uint16_t dev_addr, uint16_t addr, uint8_t *buff, uint32_t len)
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

    I2CMasterSlaveAddrSet(I2C1_BASE, dev_addr, false);
    I2CMasterDataPut(I2C1_BASE, (uint8_t)(addr >> 8));
    I2CMasterControl(I2C1_BASE, I2C_MCS_START | I2C_MCS_RUN);
    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterDataPut(I2C1_BASE, (uint8_t)addr);
    I2CMasterControl(I2C1_BASE, I2C_MCS_RUN);
    while(I2CMasterBusy(I2C1_BASE));

    if (len == 1)
    {
        I2CMasterSlaveAddrSet(I2C1_BASE, dev_addr, true);
        I2CMasterControl(I2C1_BASE, I2C_MCS_START | I2C_MCS_STOP | I2C_MCS_RUN);
        while(I2CMasterBusy(I2C1_BASE));
        *buff = I2CMasterDataGet(I2C1_BASE);
        while(I2CMasterBusy(I2C1_BASE));
   }
    else
    {
        l = len;
        I2CMasterSlaveAddrSet(I2C1_BASE, dev_addr, true);
        I2CMasterControl(I2C1_BASE, I2C_MCS_ACK | I2C_MCS_START | I2C_MCS_RUN);
        while(I2CMasterBusy(I2C1_BASE));
        *buff++ = I2CMasterDataGet(I2C1_BASE);
        while(I2CMasterBusy(I2C1_BASE));

        while(l--)
        {
            if (l == 0)
            {
                I2CMasterControl(I2C1_BASE, I2C_MCS_STOP | I2C_MCS_RUN);
            }
            else
            {
                I2CMasterControl(I2C1_BASE, I2C_MCS_ACK | I2C_MCS_RUN);
            }
            while(I2CMasterBusy(I2C1_BASE));
            *buff++ = I2CMasterDataGet(I2C1_BASE);
        }
    }

    return len;
}

uint32_t I2C_Write(uint16_t dev_addr, uint16_t addr, uint8_t *buff, uint32_t len)
{
//	uint32_t l = len;

    uint16_t endaddr;

    if(addr + len <= (addr | (PAGE_SIZE_24C64 - 1)))
    {
        endaddr = addr + len;
    }
    else
    {
        endaddr = (addr | (PAGE_SIZE_24C64 - 1)) + 1;
    }

    uint32_t l = endaddr - addr;

    I2CMasterSlaveAddrSet(I2C1_BASE, dev_addr, false);
    I2CMasterDataPut(I2C1_BASE, (uint8_t)(addr >> 8));
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterDataPut(I2C1_BASE, (uint8_t)addr);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
    while(I2CMasterBusy(I2C1_BASE));

    uint32_t result = 0;
    while(l--)
    {
        I2CMasterDataPut(I2C1_BASE, *buff++);
        result++;
        if (l == 0)
        {
            I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        }
        else
        {
            I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
        }
        while(I2CMasterBusy(I2C1_BASE));
    }
//    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_STOP);
//    while(I2CMasterBusy(I2C1_BASE));

    return result;
}

uint32_t Read24x64(uint16_t addr, uint8_t *buff, uint32_t len)
{
    return I2C_Read(DEV_ADDR_24C64, addr, buff, len);
}

uint32_t Write24x64(uint16_t addr, uint8_t *buff, uint32_t len)
{
    uint32_t rv = 0;
    uint32_t total = 0;

    do
    {
//        rv = ee24xx_write_page(eeaddr, len, buf);
        rv = I2C_Write(DEV_ADDR_24C64, addr, buff, len);
        delay_ms(4);
        if(rv == 0)
        {
            return 0;
        }

        addr += rv;
        len -= rv;
        buff += rv;
        total += rv;
    }
    while(len > 0);

    return total;
}
