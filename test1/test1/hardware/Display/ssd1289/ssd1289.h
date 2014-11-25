/*
 * ssd1289.h
 *
 *  Created on: 24 но€б. 2014 г.
 *      Author: john
 */

#ifndef SSD1289_H_
#define SSD1289_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "../../../global.h"

#define TFT_WIDTH           239
#define TFT_HEIGHT          319

#define LCD_DataPortH   GPIO_PORTB_BASE

#define TFT_CS_PORT         GPIO_PORTE_BASE
#define TFT_CS_PERIPH       SYSCTL_PERIPH_GPIOE
#define TFT_CS_PIN          GPIO_PIN_3
#define TFT_RST_PORT        GPIO_PORTE_BASE
#define TFT_RST_PERIPH      SYSCTL_PERIPH_GPIOE
#define TFT_RST_PIN         GPIO_PIN_2
#define TFT_D_C_PORT        GPIO_PORTE_BASE
#define TFT_D_C_PERIPH      SYSCTL_PERIPH_GPIOE
#define TFT_D_C_PIN         GPIO_PIN_1

#define TFT_CS_LOW      GPIOPinWrite(TFT_CS_PORT, TFT_CS_PIN, 0)
#define TFT_CS_HIGH     GPIOPinWrite(TFT_CS_PORT, TFT_CS_PIN, TFT_CS_PIN)

#define TFT_RST_ON      GPIOPinWrite(TFT_RST_PORT, TFT_RST_PIN, 0)
#define TFT_RST_OFF     GPIOPinWrite(TFT_RST_PORT, TFT_RST_PIN, TFT_RST_PIN)

#define TFT_DC_LOW      GPIOPinWrite(TFT_D_C_PORT, TFT_D_C_PIN, 0)
#define TFT_DC_HIGH     GPIOPinWrite(TFT_D_C_PORT, TFT_D_C_PIN, TFT_D_C_PIN)


//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
//extern void InitDisplay(void);
//extern void PixelDraw(void *pvDisplayData, int32_t x, int32_t y, uint32_t color);
//extern void LineDrawH(void *pvDisplayData, int32_t x1, int32_t x2, int32_t y, uint32_t color);
//extern void LineDrawV(void *pvDisplayData, int32_t x, int32_t y1, int32_t y2, uint32_t color);
//extern uint32_t ColorTranslate(void *pvDisplayData, uint32_t ui32Value);
extern const tDisplay psDisplay;


#endif /* SSD1289_H_ */
