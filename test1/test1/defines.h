/*
 * defines.h
 *
 *  Created on: 20 θώνÿ 2014 γ.
 *      Author: john
 */

#ifndef DEFINES_H_
#define DEFINES_H_

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

#define SHUTDOWN_PIN            GPIO_PIN_4
#define SHUTDOWN_PORT			GPIO_PORTE_BASE
#define SHUTDOWN_PERIPH			SYSCTL_PERIPH_GPIOE

//one wire defines
#define OW_PERIPH               SYSCTL_PERIPH_GPIOA
#define OW_PORT                 GPIO_PORTA_BASE
#define OW_PIN                  GPIO_PIN_5

//SPI
#define SPI_SPEED               30000000UL

//temperature sensors defines


//RTC defines


//EEPROM defines
#define INTERNAL_EEPROM_DEVICE_ADDRESS	0x400AF000

//display defines
#define DISPLAY_CS_PORT         GPIO_PORTE_BASE
#define DISPLAY_CS_PERIPH       SYSCTL_PERIPH_GPIOE
#define DISPLAY_CS_PIN          GPIO_PIN_3
#define DISPLAY_RST_PORT        GPIO_PORTE_BASE
#define DISPLAY_RST_PERIPH      SYSCTL_PERIPH_GPIOE
#define DISPLAY_RST_PIN         GPIO_PIN_2
#define DISPLAY_D_C_PORT        GPIO_PORTE_BASE
#define DISPLAY_D_C_PERIPH      SYSCTL_PERIPH_GPIOE
#define DISPLAY_D_C_PIN         GPIO_PIN_1

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

#endif /* DEFINES_H_ */
