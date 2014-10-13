#ifndef DS18x20_H
#define DS18x20_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define THERM_CMD_CONVERTTEMP 		0x44
#define THERM_CMD_READ_SCRATCHPAD 	0xBE
#define THERM_CMD_WRITE_SCRATCHPAD 	0x4E
#define THERM_CMD_COPY_SCRATCHPAD 	0x48
#define THERM_CMD_RELOAD_EEPROM 	0xB8
#define THERM_CMD_POWER_SUPPLY 		0xB4
//#define THERM_CMD_ALARMSEARCH 		0xEC
#define THERM_DECIMAL_STEPS_12BIT 	625 //.0625

#define DS18X20_CHECK_CRC

//int8_t DS18x20_StartMeasure(void);
//int8_t DS18x20_StartMeasureAddressed(uint8_t* rom);
//int8_t DS18x20_ReadData(uint8_t *rom, uint8_t *buffer);
//void DS18x20_ConvertToThemperature(uint8_t* data, uint8_t* themp);
void SearchTempSensors(void);
void MeasureTemperature(void * params);
char* GetTemperatureString(uint8_t devNumber);
uint8_t GetTempSensorsCount(void);

#endif
