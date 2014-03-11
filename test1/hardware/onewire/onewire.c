/*
 * onewire.c
 *
 *  Created on: 10.03.2014
 *      Author: john
 */

#include "../../global.h"

extern void RunTasks(uint8_t tasks);

#define OWPORTDIR P2DIR
#define OWPORTOUT P2OUT
#define OWPORTIN P2IN
#define OWPORTREN P2REN
#define OWPORTPIN BIT1

uint8_t owDevicesIDs[MAXDEVICES][8];

uint8_t search_ow_devices(void) // поиск всех устройств на шине
{
	uint8_t i;
	uint8_t id[OW_ROMCODE_SIZE];
	uint8_t diff, sensors_count;

	sensors_count = 0;

	for(diff = OW_SEARCH_FIRST; diff != OW_LAST_DEVICE && sensors_count < MAXDEVICES;)
	{
		OW_FindROM(&diff, &id[0]);

		if(diff == OW_PRESENCE_ERR)
		{
			break;
		}

		if(diff == OW_DATA_ERR)
		{
			break;
		}

		for (i = 0; i < OW_ROMCODE_SIZE; i++)
		{
			owDevicesIDs[sensors_count][i] = id[i];
		}

		sensors_count++;
	}
	return sensors_count;
}

void identify_ow_devices()
{
	uint8_t i = 0;
	uint8_t nDevices = search_ow_devices();
	for (; i < nDevices; i++) // теперь сотируем устройства и запрашиваем данные
	{
	// узнать устройство можно по его груповому коду, который расположен в первом байте адресса
		switch (owDevicesIDs[i][0])
		{
			case OW_DS18B20_FAMILY_CODE:
			{ // если найден термодатчик DS18B20
				//printf("\r"); print_address(owDevicesIDs[i]); // печатаем знак переноса строки, затем - адрес
				//printf(" - Thermometer DS18B20"); // печатаем тип устройства
//				DS18x20_StartMeasureAddressed(owDevicesIDs[i]); // запускаем измерение
				//timerDelayMs(800); // ждем минимум 750 мс, пока конвентируется температура
//				_delay_ms(800); // ждем минимум 750 мс, пока конвентируется температура
				SysCtlDelay(800);
				uint8_t data[2]; // переменная для хранения старшего и младшего байта данных
//				DS18x20_ReadData(owDevicesIDs[i], data); // считываем данные
				uint8_t themperature[3]; // в этот массив будет записана температура
//				DS18x20_ConvertToThemperature(data, themperature); // преобразовываем температуру в человекопонятный вид
//				ks0108GotoXY(0, 10 * i);
//				ks0108FillRect(ks0108StringWidth("value: "), 10 * i, 10, 12, WHITE);
//				sprintf(tmp, "value: %c%d.%1d C", themperature[0], themperature[1], themperature[2]);
//				ks0108Puts((char*)&tmp);
			} break;
		}
	};
}

void OthersTasks(void)
{
//	RunTasks(0xFF);
}

void OW_Set(uint8_t mode)
{
#ifndef OW_TWO_PINS
	if (mode)
	{
//		cb(OW_PORT, OW_BIT);
//		sb(OW_DDR, OW_BIT);
	}
	else
	{
//		cb(OW_PORT, OW_BIT);
//		cb(OW_DDR, OW_BIT);
	}
#else
	if (mode)
	{
//		cb(OW_PORT, OW_BIT_OUT);
	}
	else
	{
//		sb(OW_PORT, OW_BIT_OUT);
	}
#endif
}

uint8_t OW_CheckIn(void)
{
#ifndef OW_TWO_PINS
	return CheckBit(OW_PIN, OW_BIT);
#else
	return CheckBit(OW_PIN, OW_BIT_IN);
#endif
	return 0;
}

uint8_t OW_Reset(void)
{
	OW_Set(1);
	SysCtlDelay(480);
	OW_Set(0);
	SysCtlDelay(60);
	//Store line value and wait until the completion of 480uS period
	uint8_t status = OW_CheckIn();
	SysCtlDelay(420);
	//Return the value read from the presence pulse (0=OK, 1=WRONG)
 return !status;
//	return 1 if found
//	return 0 if not found
}

void OW_WriteBit(uint8_t bit)
{
	//Pull line low for 1uS
	OW_Set(1);
	SysCtlDelay(1);
//If we want to write 1, release the line (if not will keep low)
	if (bit)
	{
		OW_Set(0);
	}
	//Wait for 60uS and release the line
	SysCtlDelay(60);
	OW_Set(0);
}

uint8_t OW_ReadBit(void)
{
	uint8_t bit = 0;
	//Pull line low for 1uS
	OW_Set(1);
//	_delay_us(1);
	SysCtlDelay(1);
	//Release line and wait for 14uS
	OW_Set(0);
//	_delay_us(14);
	SysCtlDelay(14);
	//Read line value
	if (OW_CheckIn())
	{
		bit = 1;
	}
	//Wait for 45uS to end and return read value
//	_delay_us(45);
	SysCtlDelay(45);
	return bit;
//#endif
}

void OW_WriteByte(uint8_t byte)
{
	uint8_t i = 0;
	for (; i < 8; i++)
	{
		OW_WriteBit(CheckBit(byte, i));
	}
}

uint8_t OW_ReadByte(void)
{
	uint8_t n = 0;
	uint8_t i = 0;
	for (; i < 8; i++)
	{
		if (OW_ReadBit())
		{
//			sb(n, i);
		}
	}

	return n;
}

uint8_t OW_SearchROM(uint8_t diff, uint8_t *id)
{
	uint8_t i, j, next_diff;
	uint8_t b;

	if (!OW_Reset())
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
			if (OW_ReadBit())
			{ // read complement bit
				if (b)                 // 11
				{
					return OW_DATA_ERR;  // data error
				}
			}
			else
			{
				if (!b)
				{ // 00 = 2 devices
					if (diff > i || ((*id & 1) && diff != i))
					{
						b = 1;               // now 1
						next_diff = i;       // next pass 0
					}
				}
			}
			OW_WriteBit(b);               // write bit
			*id >>= 1;
			if (b)
			{
				*id |= 0x80;			// store bit
			}
			i--;
		} while (--j);
		id++;                            // next byte
	} while (i);
	return next_diff;                  // to continue search
}

void OW_FindROM(uint8_t *diff, uint8_t id[])
{
	while (1)
	{
		*diff = OW_SearchROM(*diff, &id[0]);
		if (*diff == OW_PRESENCE_ERR || *diff == OW_DATA_ERR
				|| *diff == OW_LAST_DEVICE)
		{
			return;
		}
		//if ( id[0] == DS18B20_ID || id[0] == DS18S20_ID )
		return;
	}
}

uint8_t OW_ReadROM(uint8_t *buffer)
{
	if (!OW_Reset())
	{
		return 0;
	}
	OW_WriteByte(OW_CMD_READROM);
	uint8_t i = 0;
	for (; i < 8; i++)
	{
		buffer[i] = OW_ReadByte();
	}
	return 1;
}

uint8_t OW_MatchROM(uint8_t *rom)
{
	if (!OW_Reset())
	{
		return 0;
	}
	OW_WriteByte(OW_CMD_MATCHROM);
	uint8_t i = 0;
	for (; i < 8; i++)
	{
		OW_WriteByte(rom[i]);
	}
	return 1;
}
