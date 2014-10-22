/*
 * onewire.c
 *
 *  Created on: 10.03.2014
 *      Author: john
 */

#include "../../global.h"

void OW_Init()
{
    SysCtlPeripheralEnable(OW_PERIPH);
    GPIOPadConfigSet(OW_PORT, OW_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
//    GPIOPinTypeGPIOOutputOD(OW_PORT, OW_PIN);
//    GPIOPinTypeGPIOOutput(OW_PORT, OW_PIN);
}

void OW_Set(ow_enum mode)
{
    if(mode == OW_OUT)
    {
        GPIODirModeSet(OW_PORT, OW_PIN, GPIO_DIR_MODE_OUT);
    }
    else
    {
        GPIODirModeSet(OW_PORT, OW_PIN, GPIO_DIR_MODE_IN);
    }
    GPIOPinWrite(OW_PORT, OW_PIN, 0);
}

int32_t OW_CheckIn(void)
{
    int32_t pinRead = GPIOPinRead(OW_PORT, OW_PIN);
    int32_t result = ((pinRead & OW_PIN) == OW_PIN);
    return result;
}

uint8_t OW_Reset(void)
{
    OW_Set(OW_OUT);
    delayMicroseconds(485);
    OW_Set(OW_IN);
    delayMicroseconds(65);
    uint8_t status = OW_CheckIn();
    delayMicroseconds(420);
    status = OW_CheckIn();

    return status;
//
//    if(status != 0)
//    {
//        return 1;
//    }
//    //Return the value read from the presence pulse (0=OK, 1=WRONG)
//    return !status;
}

void OW_WriteBit(uint8_t bit)
{
    //Pull line low for 1uS
    OW_Set(OW_OUT);
    delayMicroseconds(1);
    //If we want to write 1, release the line (if not will keep low)
    if(bit)
    {
        OW_Set(OW_IN);
    }
    //Wait for 60uS and release the line
    delayMicroseconds(60);
    OW_Set(OW_IN);
}

uint8_t OW_ReadBit(void)
{
    uint8_t bit = 0;
    //Pull line low for 1uS
    OW_Set(OW_OUT);
    delayMicroseconds(1);
    //Release line and wait for 14uS
    OW_Set(OW_IN);
    delayMicroseconds(14);
    //Read line value
    if(OW_CheckIn())
    {
        bit = 1;
    }
    //Wait for 45uS to end and return read value
    delayMicroseconds(45);
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
    uint8_t j;
    uint8_t next_diff;
    uint8_t bit;

    if(!OW_Reset())
    {
        return OW_PRESENCE_ERR;       // error, no device found
    }

    OW_WriteByte(OW_CMD_SEARCHROM);     // ROM search command
    next_diff = OW_LAST_DEVICE;      // unchanged on last device

    uint8_t i = OW_ROMCODE_SIZE * 8;         // 8 bytes
    do
    {
        j = 8;                        // 8 bits
        do
        {
            bit = OW_ReadBit();			// read bit
            if(OW_ReadBit())
            { // read complement bit
                if(bit)                 // 11
                {
                    return OW_DATA_ERR;  // data error
                }
            }
            else
            {
                if(!bit)
                { // 00 = 2 devices
                    if(diff > i || ((*id & 1) && diff != i))
                    {
                        bit = 1;               // now 1
                        next_diff = i;       // next pass 0
                    }
                }
            }
            OW_WriteBit(bit);               // write bit
            *id >>= 1;
            if(bit)
            {
                *id |= 0x80;			// store bit
            }
            i--;
        } while(--j);
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

uint8_t OW_SetPrecision(uint8_t p)
{
//    ow_write_byte(0x4E) //send command to write scratchpad
//    ow_write_byte(0xFF) //buffer TH
//    ow_write_byte(0xFF) //buffer TL
//    ow_write_byte(0x00) //send 0 to config register
    return 0;
}
