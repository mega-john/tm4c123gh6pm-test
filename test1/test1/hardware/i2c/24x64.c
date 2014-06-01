/*
 * 24x64.c
 *
 *  Created on: 20 мая 2014 г.
 *      Author: john
 */

#include "24x64.h"

static void CheckI2CResult(int line, const char* f)
{
#ifdef DEBUG
    uint32_t error = I2CMasterErr(I2C1_BASE);
    if (error != 0)
    {
        UARTprintf("\r%s line: %i, error code: %i", f, line, error);

//        UARTprintf("Send Zero Error:\n");
//        switch (error)
//        {
//        case I2C_MASTER_ERR_ADDR_ACK:
//            UARTprintf("1 I2C_MASTER_ERR_ADDR_ACK\n");
//            break;
//        case I2C_MASTER_ERR_DATA_ACK:
//            UARTprintf("2 I2C_MASTER_ERR_DATA_ACK\n");
//            break;
//        case I2C_MASTER_ERR_ARB_LOST:
//            UARTprintf("3 I2C_MASTER_ERR_ARB_LOST\n");
//            break;
//        default:
//            UARTprintf("WTF: %d\n", error);
//        }

    }
#else
    return;
#endif
}

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
    CheckI2CResult(__LINE__, __FILE__);
    I2CMasterDataPut(I2C1_BASE, (uint8_t)(addr >> 8));
    CheckI2CResult(__LINE__, __FILE__);
    I2CMasterControl(I2C1_BASE, I2C_MCS_START | I2C_MCS_RUN);
    CheckI2CResult(__LINE__, __FILE__);
    while(I2CMasterBusy(I2C1_BASE));
    CheckI2CResult(__LINE__, __FILE__);

    I2CMasterDataPut(I2C1_BASE, (uint8_t)addr);
    CheckI2CResult(__LINE__, __FILE__);
    I2CMasterControl(I2C1_BASE, I2C_MCS_RUN);
    CheckI2CResult(__LINE__, __FILE__);
    while(I2CMasterBusy(I2C1_BASE));
    CheckI2CResult(__LINE__, __FILE__);

    if (len == 1)
    {
        I2CMasterSlaveAddrSet(I2C1_BASE, dev_addr, true);
        CheckI2CResult(__LINE__, __FILE__);
        I2CMasterControl(I2C1_BASE, I2C_MCS_START | I2C_MCS_STOP | I2C_MCS_RUN);
        while (I2CMasterBusy(I2C1_BASE));
        CheckI2CResult(__LINE__, __FILE__);
        *buff = I2CMasterDataGet(I2C1_BASE);
        CheckI2CResult(__LINE__, __FILE__);
        while (I2CMasterBusy(I2C1_BASE));
        CheckI2CResult(__LINE__, __FILE__);
   }
    else
    {
        l = len;
        I2CMasterSlaveAddrSet(I2C1_BASE, dev_addr, true);
        CheckI2CResult(__LINE__, __FILE__);
        I2CMasterControl(I2C1_BASE, I2C_MCS_ACK | I2C_MCS_START | I2C_MCS_RUN);
        CheckI2CResult(__LINE__, __FILE__);
        while(I2CMasterBusy(I2C1_BASE));
        CheckI2CResult(__LINE__, __FILE__);
        *buff++ = I2CMasterDataGet(I2C1_BASE);
        CheckI2CResult(__LINE__, __FILE__);
        while(I2CMasterBusy(I2C1_BASE));
        CheckI2CResult(__LINE__, __FILE__);

        while(l--)
        {
            if (l == 0)
            {
                I2CMasterControl(I2C1_BASE, I2C_MCS_STOP | I2C_MCS_RUN);
                CheckI2CResult(__LINE__, __FILE__);
           }
            else
            {
                I2CMasterControl(I2C1_BASE, I2C_MCS_ACK | I2C_MCS_RUN);
                CheckI2CResult(__LINE__, __FILE__);
           }
            while(I2CMasterBusy(I2C1_BASE));
            *buff++ = I2CMasterDataGet(I2C1_BASE);
            CheckI2CResult(__LINE__, __FILE__);
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
    CheckI2CResult(__LINE__, __FILE__);
    I2CMasterDataPut(I2C1_BASE, (uint8_t)(addr >> 8));
    CheckI2CResult(__LINE__, __FILE__);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    CheckI2CResult(__LINE__, __FILE__);
    while(I2CMasterBusy(I2C1_BASE));
    CheckI2CResult(__LINE__, __FILE__);

    I2CMasterDataPut(I2C1_BASE, (uint8_t)addr);
    CheckI2CResult(__LINE__, __FILE__);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
    CheckI2CResult(__LINE__, __FILE__);
    while(I2CMasterBusy(I2C1_BASE));
    CheckI2CResult(__LINE__, __FILE__);

    uint32_t result = 0;
    while(l--)
    {
        I2CMasterDataPut(I2C1_BASE, *buff++);
        CheckI2CResult(__LINE__, __FILE__);
        result++;
        if (l == 0)
        {
            I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
            CheckI2CResult(__LINE__, __FILE__);
       }
        else
        {
            I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
            CheckI2CResult(__LINE__, __FILE__);
        }
        while(I2CMasterBusy(I2C1_BASE));
        CheckI2CResult(__LINE__, __FILE__);
   }

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
        UARTprintf("\rI2C_Write addr: %i, buff: %i, len: %i", addr, buff, len);
        rv = I2C_Write(DEV_ADDR_24C64, addr, buff, len);
//        delay_ms(4);
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
