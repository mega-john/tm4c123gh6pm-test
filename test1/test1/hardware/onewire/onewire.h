/*
 * onewire.h
 *
 *  Created on: 10.03.2014
 *      Author: john
 */
#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include "../../global.h"

typedef enum
{
    OW_IN = 0, OW_OUT = 1
} ow_enum;

#define OW_CMD_SEARCHROM        0xF0
#define OW_CMD_READROM			0x33
#define OW_CMD_MATCHROM         0x55
#define OW_CMD_SKIPROM          0xCC
#define OW_CMD_ALARMSEARCH      0xEC

#define	OW_SEARCH_FIRST			0xFF
#define	OW_PRESENCE_ERR			0xFF
#define	OW_DATA_ERR				0xFE
#define OW_LAST_DEVICE			0x00
#define OW_DS1990_FAMILY_CODE	1
#define OW_DS2405_FAMILY_CODE	5
#define OW_DS2413_FAMILY_CODE	0x3A
#define OW_DS1822_FAMILY_CODE	0x22
#define OW_DS2430_FAMILY_CODE	0x14
#define OW_DS1990_FAMILY_CODE	1
#define OW_DS2431_FAMILY_CODE	0x2D
#define OW_DS18S20_FAMILY_CODE	0x10
#define OW_DS18B20_FAMILY_CODE	0x28
#define OW_DS2433_FAMILY_CODE	0x23

// rom-code size including CRC
#define OW_ROMCODE_SIZE			8

// Если для эмуляции шины используется USART
//#define UART_AS_OneWire

void OW_Init();

uint8_t OW_Reset(void);
void OW_WriteBit(uint8_t bit);
uint8_t OW_ReadBit(void);

#ifndef UART_AS_OneWire
    uint8_t OW_ReadByte(void);
    void OW_WriteByte(uint8_t byte);
#else
    uint8_t OW_WriteByte(uint8_t byte);
    #define OW_ReadByte() OW_WriteByte(0xFF)
#endif


//uint8_t OW_ReadByte(void);
//void OW_WriteByte(uint8_t byte);

uint8_t OW_SearchROM(uint8_t diff, uint8_t *id);
void OW_FindROM(uint8_t *diff, uint8_t id[]);
uint8_t OW_ReadROM(uint8_t *buffer);
uint8_t OW_MatchROM(uint8_t *rom);

#endif /* ONEWIRE_H_ */
