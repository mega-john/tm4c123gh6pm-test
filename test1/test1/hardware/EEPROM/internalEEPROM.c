/*
 * internalEEPROM.c
 *
 *  Created on: 12 рту. 2014 у.
 *      Author: estarcev
 */
#include "internalEEPROM.h"

void InitInternalEEPROM()
{
	//
	// Enable the EEPROM peripheral.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);

	//
	// Initialize the EEPROM
	//
	EEPROMInit();
}

//void InternalEEPROMWrite(unsigned char * pucBuffer, int iLength, int iOffset)
//{
//	//
//	// Write the info to the EEPROM.
//	//
//	EEPROMProgram((uint32_t *) pucBuffer, (uint32_t) (0 + iOffset),
//			(uint32_t) iLength);
//}
//
//void InternalEEPROMRead(unsigned char * pucBuffer, int iLength, int iOffset)
//{
//	//
//	// Read the requested data.
//	//
//	EEPROMRead((uint32_t *) pucBuffer, (uint32_t) (0 + iOffset),
//			(uint32_t) iLength);
//}

void LoadInt32(int32_t * value, uint32_t addr)
{
	EEPROMRead((uint32_t *)value, addr, sizeof(int32_t));
}

void SaveInt32(int32_t * value, uint32_t addr)
{
	EEPROMProgram((uint32_t *)value, addr, sizeof(int32_t));
}

void LoadUInt32(uint32_t * value, uint32_t addr)
{
	EEPROMRead(value, addr, sizeof(int32_t));
}

void SaveUInt32(uint32_t * value, uint32_t addr)
{
	EEPROMProgram(value, addr, sizeof(int32_t));
}

void LoadInt16(int16_t * value, uint32_t addr)
{
	LoadInt32((int32_t *)value, addr);
}

void SaveInt16(int16_t * value, uint32_t addr)
{
	SaveInt32((int32_t *)value, addr);
}

void LoadUInt16(uint16_t * value, uint32_t addr)
{
	LoadUInt32((uint32_t *)value, addr);
}

void SaveUInt16(uint16_t * value, uint32_t addr)
{
	SaveUInt32((uint32_t *)value, addr);
}

void LoadInt8(int8_t * value, uint32_t addr)
{
	LoadInt32((int32_t *)value, addr);
}

void SaveInt8(int8_t * value, uint32_t addr)
{
	SaveInt32((int32_t *)value, addr);
}

void LoadUInt8(uint8_t * value, uint32_t addr)
{
	LoadUInt32((uint32_t *)value, addr);
}

void SaveUInt8(uint8_t * value, uint32_t addr)
{
	SaveUInt32((uint32_t *)value, addr);
}

void LoadFloat(float * value, uint32_t addr)
{
	EEPROMRead((uint32_t*)value, addr, sizeof(float));
}

void SaveFloat(float * value, uint32_t addr)
{
	EEPROMProgram((uint32_t*)value, addr, sizeof(float));
}

void LoadDouble(double * value, uint32_t addr)
{
	EEPROMRead((uint32_t*)value, addr, sizeof(double));
}

void SaveDouble(double * value, uint32_t addr)
{
	EEPROMProgram((uint32_t*)value, addr, sizeof(double));
}

void LoadArray(uint8_t * array, uint16_t size, uint32_t addr)
{
	EEPROMRead((uint32_t*)array, addr, size);
}

void SaveArray(uint8_t * array, uint16_t size, uint32_t addr)
{
	EEPROMProgram((uint32_t*)array, addr, size);
}

void TestInternalEEPROM()
{
	UARTprintf("\n init internal EEPROM");
	InitInternalEEPROM();

	UARTprintf("\n init internal EEPROM complete");

	uint8_t su8 = 0xab;
	SaveUInt8(&su8, 0);
	uint32_t su32 = 0xabcdefab;
	SaveUInt32(&su32, 4);
	uint16_t su16 = 0xabcd;
	SaveUInt16(&su16, 8);
	int8_t si8 = -123;
	SaveInt8(&si8, 16);
	int32_t si32 = -123456789;
	SaveInt32(&si32, 20);
	int16_t si16 = 12345;
	SaveInt16(&si16, 24);
	float sf = -1.1267;
	SaveFloat(&sf, 28);
	double sd = 1234.4575678567;
	SaveDouble(&sd, 32);
//	SaveArray();

	uint8_t lu8 = 0;
	LoadUInt8(&lu8, 0);
	uint32_t lu32 = 0;
	LoadUInt32(&lu32, 4);
	uint16_t lu16 = 0;
	LoadUInt16(&lu16, 8);
	int8_t li8 = 0;
	LoadInt8(&li8, 16);
	int32_t li32 = 0;
	LoadInt32(&li32, 20);
	int16_t li16 = 0;
	LoadInt16(&li16, 24);
	float lf = 0;
	LoadFloat(&lf, 28);
	double ld = 0;
	LoadDouble(&ld, 32);
//	LoadArray();

//	EEPROMMassErase();
//	unsigned long pulRead[2];
//	int writeData=0xabcdefff;
//	const unsigned char writeData[6] = {0x11,0x22,0x33,0x44,0x55,0x66};
//	int size = sizeof(writeData) + 2;

//	UARTprintf("\n write to internal EEPROM");
//	InternalEEPROMWrite((unsigned char *)writeData, size, 0);
//	unsigned char readData[6] = {0};
//	UARTprintf("\n read from internal EEPROM");
//	InternalEEPROMRead((unsigned char *)readData, size, 0);
//	uint8_t i = 0;
//	UARTprintf("\n result: ", readData[i]);
//	for(;i < size; i++)
//	{
//		UARTprintf("0x%x ", readData[i]);
//	}

//	float f = 10.123456789;
//	char buf[20];
//	_ftoa(f, buf, 4);
//	UARTprintf("\n write to internal EEPROM float %s", buf);
//	InternalEEPROMWrite((unsigned char *)&f, sizeof(float), 0);
//	UARTprintf("\n read from internal EEPROM");
//	float ff = 0;
//	InternalEEPROMRead((unsigned char *)&ff, sizeof(float), 0);
//	_ftoa(ff, buf, 4);
//	UARTprintf("\n result: %s", buf);
}
