/*
 * onewire.c
 *
 *  Created on: 10.03.2014
 *      Author: john
 */

#include "../../global.h"
#include "onewire.h"

extern void RunTasks(uint8_t tasks);

void OthersTasks(void)
{
//	RunTasks(0xFF);
}

static uint32_t OW_PERIPH = 0;
static uint32_t OW_PORT = 0;
static uint8_t OW_PIN = 0;

void OW_Init(uint32_t periph, uint32_t portBase, uint8_t pin)
{
    OW_PERIPH = periph;
    OW_PORT = portBase;
    OW_PIN = pin;

    SysCtlPeripheralEnable(OW_PERIPH);
    GPIODirModeSet(OW_PORT, OW_PIN, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(OW_PORT, OW_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

/*
Если в DDRxn записан 1, то Pxn работает как выход, если 0, то как вход.

Если в PORTxn записывается 1 и ножка Pxn сконфигурирована как вход, то активируется pull-up резистор.
Чтобы отключить этот резистор, нужно записать в PORTxn лог. 0 или сконфигурировать ножку Pxn как выход.
Ножка порта находится в третьем (отключенном) состоянии, когда активен сброс, а также если отсутствует тактовый сигнал.
Если в PORTxn записывается 1 и ножка Pxn сконфигурирована как выход, то включается верхний ключ Pxn (на выходе 1),
если в PORTxn записывается 0, то включается нижний ключ (на выходе 0).
Записью лог. 1 в PINxn переключает величину PORTxn на противоположную, независимо от состояния DDRxn.
Таким образом, инструкция SBI может использоваться для смены состояния одиночного бита порта.

Переключение режима порта между входом и выходом.
Когда происходит переключение от третьего состояния ({DDxn, PORTxn} = 0b00) до выходного состояния high ({DDxn, PORTxn} = 0b11),
может с равной вероятностью произойти промежуточное состояние либо разрешенное pull-up ({DDxn, PORTxn} = 0b01),
либо выходной ноль ({DDxn, PORTxn} = 0b10).

Переключение между входом с pull-up и выходом low дает аналогичную проблему.
Чтобы избежать этого, пользователь должен использовать промежуточное состояние - либо третье состояние ({DDxn, PORTxn} = 0b00),
либо выходное состояние high ({DDxn, PORTxn} = 0b10).
 */

void OW_Set(ow_enum mode)
{
    if (mode == OW_OUT)
    {
//		cb(OW_PORT, OW_BIT);
//		sb(OW_DDR, OW_BIT);//set to outuput
        GPIOPadConfigSet(OW_PORT, OW_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
        GPIODirModeSet(OW_PORT, OW_PIN, GPIO_DIR_MODE_OUT);
        GPIOPinWrite(OW_PORT, OW_PIN, 0);
    }
    else
    {
//		cb(OW_PORT, OW_BIT);
//		cb(OW_DDR, OW_BIT);//set to input & disable pull-up resistor
//        GPIOPinWrite(OW_PORT, OW_PIN, 0);
        GPIOPadConfigSet(OW_PORT, OW_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD);
        GPIODirModeSet(OW_PORT, OW_PIN, GPIO_DIR_MODE_IN);
    }
}

uint8_t OW_CheckIn(void)
{
    return (GPIOPinRead(OW_PORT, OW_PIN) & OW_PIN);
}

uint8_t OW_Reset(void)
{
    OW_Set(OW_OUT);
    delay_us(480);
    OW_Set(OW_IN);
    delay_us(60);
    //Store line value and wait until the completion of 480uS period
    uint8_t status = OW_CheckIn();
    delay_us(420);
    //Return the value read from the presence pulse (0=OK, 1=WRONG)
    return !status;
    //	return 1 if found
    //	return 0 if not found
}

void OW_WriteBit(uint8_t bit)
{
    //Pull line low for 1uS
    OW_Set(OW_OUT);
    delay_us(1);
    //If we want to write 1, release the line (if not will keep low)
    if (bit)
    {
        OW_Set(OW_IN);
    }
    //Wait for 60uS and release the line
    delay_us(60);
    OW_Set(OW_IN);
}

uint8_t OW_ReadBit(void)
{
    uint8_t bit = 0;
    //Pull line low for 1uS
    OW_Set(OW_OUT);
    delay_us(1);
    //Release line and wait for 14uS
    OW_Set(OW_IN);
    delay_us(14);
    //Read line value
    if(OW_CheckIn())
    {
        bit = 1;
    }
    //Wait for 45uS to end and return read value
    delay_us(45);
    return bit;
}

void OW_WriteByte(uint8_t byte)
{
    uint8_t i = 0;
    for(; i < 8; i++)
    {
        OW_WriteBit(CheckBit(byte, i));
    }
}

uint8_t OW_ReadByte(void)
{
    uint8_t n = 0;
    uint8_t i = 0;
    for(; i < 8; i++)
    {
        if(OW_ReadBit())
        {
			sb(n, i);
        }
    }

    return n;
}

uint8_t OW_SearchROM(uint8_t diff, uint8_t *id)
{
    uint8_t i, j, next_diff;
    uint8_t b;

    if( !OW_Reset())
    {
        return OW_PRESENCE_ERR;       // error, no device found
    }

    OW_WriteByte(OW_CMD_SEARCHROM);     // ROM search command
    next_diff = OW_LAST_DEVICE;      // unchanged on last device

    i = OW_ROMCODE_SIZE * 8;         // 8 bytes
    do
    {
        j = 8;                        // 8 bits
        do
        {
            b = OW_ReadBit();			// read bit
            if(OW_ReadBit())
            { // read complement bit
                if(b)                 // 11
                {
                    return OW_DATA_ERR;  // data error
                }
            }
            else
            {
                if( !b)
                { // 00 = 2 devices
                    if(diff > i || ((*id & 1) && diff != i))
                    {
                        b = 1;               // now 1
                        next_diff = i;       // next pass 0
                    }
                }
            }
            OW_WriteBit(b);               // write bit
            *id >>= 1;
            if(b)
            {
                *id |= 0x80;			// store bit
            }
            i--;
        } while( --j);
        id++;                            // next byte
    } while(i);
    return next_diff;                  // to continue search
}

void OW_FindROM(uint8_t *diff, uint8_t id[])
{
    while(1)
    {
        *diff = OW_SearchROM(*diff, &id[0]);
        if(*diff == OW_PRESENCE_ERR || *diff == OW_DATA_ERR || *diff == OW_LAST_DEVICE)
        {
            return;
        }
        //if ( id[0] == DS18B20_ID || id[0] == DS18S20_ID )
        return;
    }
}

uint8_t OW_ReadROM(uint8_t *buffer)
{
    if(!OW_Reset())
    {
        return 0;
    }
    OW_WriteByte(OW_CMD_READROM);
    uint8_t i = 0;
    for(; i < 8; i++)
    {
        buffer[i] = OW_ReadByte();
    }
    return 1;
}

uint8_t OW_MatchROM(uint8_t *rom)
{
    if(!OW_Reset())
    {
        return 0;
    }
    OW_WriteByte(OW_CMD_MATCHROM);
    uint8_t i = 0;
    for(; i < 8; i++)
    {
        OW_WriteByte(rom[i]);
    }
    return 1;
}
