/*
 * defines.h
 *
 *  Created on: 20 июня 2014 г.
 *      Author: john
 */

#ifndef DEFINES_H_
#define DEFINES_H_

//датчики расхода и скорости
//external interrupts defines
#define IN_FUEL_PIN             GPIO_PIN_5
#define IN_FUEL_PORT            GPIO_PORTB_BASE
#define IN_FUEL_PERIPH          SYSCTL_PERIPH_GPIOB
#define OUT_FUEL_PIN            GPIO_PIN_0
#define OUT_FUEL_PORT           GPIO_PORTB_BASE
#define OUT_FUEL_PERIPH         SYSCTL_PERIPH_GPIOB
#define IN_DISTANCE_PIN         GPIO_PIN_1
#define IN_DISTANCE_PORT        GPIO_PORTB_BASE
#define IN_DISTANCE_PERIPH      SYSCTL_PERIPH_GPIOB

//датчик выключения двигателя/питания
#define SHUTDOWN_PIN            GPIO_PIN_4
#define SHUTDOWN_PORT			GPIO_PORTE_BASE
#define SHUTDOWN_PERIPH			SYSCTL_PERIPH_GPIOE

//датчики температуры
//one wire defines
#define OW_PERIPH               SYSCTL_PERIPH_GPIOA
#define OW_PORT                 GPIO_PORTA_BASE
#define OW_PIN                  GPIO_PIN_5

//пины подключения кнопок управления
//buttons defines
#define BUTTONS_PERIPH          SYSCTL_PERIPH_GPIOF
#define BUTTONS_PORT            GPIO_PORTF_BASE
#define NUM_BUTTONS             5
#define UP_BUTTON               GPIO_PIN_0
#define DOWN_BUTTON             GPIO_PIN_4
#define LEFT_BUTTON             GPIO_PIN_3
#define RIGHT_BUTTON            GPIO_PIN_2
#define SELECT_BUTTON           GPIO_PIN_1
#define ALL_BUTTONS             (LEFT_BUTTON | RIGHT_BUTTON | UP_BUTTON |  DOWN_BUTTON | SELECT_BUTTON)

//SPI
#define SPI_SPEED               30000000UL

//temperature sensors defines


//RTC defines


//EEPROM defines
#define INTERNAL_EEPROM_DEVICE_ADDRESS	0x400AF000
#define I2C_EEPROM_SCL_PIN  GPIO_PIN_6
#define I2C_EEPROM_SDA_PIN  GPIO_PIN_7

////////////////////////////////////////////////////////////////////////////
//
// Maximum number of devices that search can be performed using this module.
//
#define MAX_DEVICES 30

#define SEARCH_ROM_COMMAND 0xF0

#define GET_OW_RIS false

//
// Path to be taken is '1'
//
#define TAKE_PATH1 1

//
// Path to be taken is '0'
//
#define TAKE_PATH0 0

//
// Polynomial: x^8+x^5+x^4+1
//
#define CRC_PLOY9 0b100110001

//
// Degree of the CRC polynomial
//
#define POLY_DEGREE 8


#endif /* DEFINES_H_ */
