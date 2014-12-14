/*
 * 1wire_uart.h
 *
 *  Created on: 10 дек. 2014 г.
 *      Author: john
 */

#ifndef WIRE_UART_H_
#define WIRE_UART_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"

#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "driverlib/udma.h"
#include "driverlib/uart.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"

// выбираем, на каком USART находится 1-wire
//#define OW_USART1
//#define OW_USART2
//#define OW_USART3
//#define OW_USART4


// если нужно отдавать тики FreeRTOS, то раскомментировать
//#define OW_GIVE_TICK_RTOS

// первый параметр функции OW_Send
#define OW_SEND_RESET   1
#define OW_NO_RESET     2

// статус возврата функций
#define OW_OK           1
#define OW_ERROR        2
#define OW_NO_DEVICE    3

#define OW_NO_READ      0xff

#define OW_READ_SLOT    0xff

extern uint8_t OW_InitUART();
extern uint8_t OW_Send(uint8_t sendReset, uint8_t *command, uint8_t cLen, uint8_t *data, uint8_t dLen, uint8_t readStart);

#endif /* WIRE_UART_H_ */
