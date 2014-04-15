#include "../../global.h"
#include "onewire.h"
#include "driverlib/onewire.h"

extern uint8_t owDevicesIDs[MAX_OW_DEVICES][8];

uint8_t search_ow_devices(void) // поиск всех устройств на шине
{
    uint8_t i;
    uint8_t id[OW_ROMCODE_SIZE];
    uint8_t diff, sensors_count;

    sensors_count = 0;

    for(diff = OW_SEARCH_FIRST; diff != OW_LAST_DEVICE && sensors_count < MAX_OW_DEVICES;)
    {
        OW_FindROM( &diff, &id[0]);

        if(diff == OW_PRESENCE_ERR)
        {
            break;
        }

        if(diff == OW_DATA_ERR)
        {
            break;
        }

        for(i = 0; i < OW_ROMCODE_SIZE; i++)
        {
            owDevicesIDs[sensors_count][i] = id[i];
        }

        sensors_count++;
    }
    return sensors_count;
}

void identify_ow_devices()
{
    OW_Init(SYSCTL_PERIPH_GPIOA, GPIO_PORTA_BASE, GPIO_PIN_5);
    OW_Reset();
    uint8_t i = 0;
    uint8_t nDevices = search_ow_devices();
    for(; i < nDevices; i++) // теперь сотируем устройства и запрашиваем данные
    {
        // узнать устройство можно по его груповому коду, который расположен в первом байте адресса
        switch(owDevicesIDs[i][0])
        {
            case OW_DS18B20_FAMILY_CODE:
            { // если найден термодатчик DS18B20
              //printf("\r"); print_address(owDevicesIDs[i]); // печатаем знак переноса строки, затем - адрес
              //printf(" - Thermometer DS18B20"); // печатаем тип устройства
                DS18x20_StartMeasureAddressed(owDevicesIDs[i]); // запускаем измерение
                //timerDelayMs(800); // ждем минимум 750 мс, пока конвентируется температура
                delay_ms(800);
                uint8_t data[2]; // переменная для хранения старшего и младшего байта данных
                DS18x20_ReadData(owDevicesIDs[i], data); // считываем данные
                uint8_t themperature[3]; // в этот массив будет записана температура
                DS18x20_ConvertToThemperature(data, themperature); // преобразовываем температуру в человекопонятный вид
//              ks0108GotoXY(0, 10 * i);
//              ks0108FillRect(ks0108StringWidth("value: "), 10 * i, 10, 12, WHITE);
//              sprintf(tmp, "value: %c%d.%1d C", themperature[0], themperature[1], themperature[2]);
//              ks0108Puts((char*)&tmp);
            }
                break;
        }
    };
}

int8_t DS18x20_StartMeasure(void)
{
	//Reset, skip ROM and start temperature conversion
	if (!OW_Reset()) 
	{
		return 0;
	}
	OW_WriteByte(OW_CMD_SKIPROM);
	OW_WriteByte(THERM_CMD_CONVERTTEMP);
	return 1;
}

int8_t DS18x20_StartMeasureAddressed(uint8_t* rom)
{
	//Reset and start temperature conversion
	if (!OW_Reset()) 
	{
		return 0;
	}
	OW_MatchROM(rom);
	OW_WriteByte(THERM_CMD_CONVERTTEMP);
	return 1;
}

#ifdef DS18X20_CHECK_CRC
#define CRC8INIT	0x00
#define CRC8POLY	0x18              //0X18 = X^8+X^5+X^4+X^0

uint8_t crc8(uint8_t *data_in, uint16_t number_of_bytes_to_read)
{
	uint8_t crc;
	uint8_t bit_counter;
	uint8_t data;
	uint8_t feedback_bit;
	uint16_t loop_count;

	crc = CRC8INIT;

	for (loop_count = 0; loop_count != number_of_bytes_to_read; loop_count++)
	{
		data = data_in[loop_count];

		bit_counter = 8;
		do
		{
			feedback_bit = (crc ^ data) & 0x01;
			if (feedback_bit == 0x01)
			{
				crc = crc ^ CRC8POLY;
			}

			crc = (crc >> 1) & 0x7F;
			if (feedback_bit == 0x01)
			{
				crc = crc | 0x80;
			}

			data = data >> 1;
			bit_counter--;
		} while (bit_counter > 0);
	}
	return crc;
}
#endif 

int8_t DS18x20_ReadData(uint8_t *rom, uint8_t *buffer)
{
	//Reset, skip ROM and send command to read Scratchpad
	if (!OW_Reset()) 
	{
		return 0;
	}
	if (rom[0] == 0)
	{
		OW_WriteByte(OW_CMD_SKIPROM);
	}
	else 
	{
		OW_MatchROM(rom);
	}
	OW_WriteByte(THERM_CMD_RSCRATCHPAD);
	
#ifdef DS18X20_CHECK_CRC
	unsigned char	buff[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	uint8_t i = 0;
	for (; i < 9; i++)
	{
		buff[i] = OW_ReadByte();
	}
	buffer[0] = buff[0]; 
	buffer[1] = buff[1];
	if (crc8(buff, 9)) 
	{
		return 0;	// если контрольная сумма не совпала, возвращаем ошибку
	}
#else 
	//Read Scratchpad (only 2 first bytes)
	buffer[0] = OW_ReadByte(); // Read TL
	buffer[1] = OW_ReadByte(); // Read TH	
#endif

	return 1;
}

void DS18x20_ConvertToThemperature(uint8_t* data, uint8_t* themp)
{
	//Store temperature integer digits and decimal digits
	themp[1] = data[0] >> 4;
	themp[1] |= (data[1] & 0x07) << 4;
	//Store decimal digits
	themp[2] = data[0] & 0xf;
	themp[2] *= 5;
	if (data[1] > 0xFB)
	{
		themp[1] = 127 - themp[1];
		themp[0] = '-';
	}
	else if ((data[0] == 0x00) && (data[1] == 0x00))
	{
		themp[0] = ' ';
	}
	else
	{
	    themp[0] = '+';
	}
}

