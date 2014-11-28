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

#define TFT_Data_Port           GPIO_PORTB_BASE
#define TFT_Data_Periph         SYSCTL_PERIPH_GPIOB

#define TFT_Command_Port        GPIO_PORTF_BASE
#define TFT_Command_Periph      SYSCTL_PERIPH_GPIOF
#define TFT_RS_PIN              GPIO_PIN_0//register select
#define TFT_WR_PIN              GPIO_PIN_1//write
//#define TFT_RD_PIN              GPIO_PIN_6//read, not used-connect to gnd
#define TFT_CS_PIN              GPIO_PIN_3//chip select
#define TFT_RST_PIN             GPIO_PIN_4//reset

#define GPIOWrite0(port, pin)   GPIOPinWrite(port, pin, ~pin)
#define GPIOWrite1(port, pin)   GPIOPinWrite(port, pin, pin)

#define TFT_RS_LOW              GPIOWrite0(TFT_Command_Port, TFT_RS_PIN)
#define TFT_RS_HIGH             GPIOWrite1(TFT_Command_Port, TFT_RS_PIN)
#define TFT_WR_LOW              GPIOWrite0(TFT_Command_Port, TFT_WR_PIN)
#define TFT_WR_HIGH             GPIOWrite1(TFT_Command_Port, TFT_WR_PIN)
#define TFT_RD_LOW              GPIOWrite0(TFT_Command_Port, TFT_RD_PIN)
#define TFT_RD_HIGH             GPIOWrite1(TFT_Command_Port, TFT_RD_PIN)
#define TFT_CS_LOW              GPIOWrite0(TFT_Command_Port, TFT_CS_PIN)
#define TFT_CS_HIGH             GPIOWrite1(TFT_Command_Port, TFT_CS_PIN)
#define TFT_RST_LOW             GPIOWrite0(TFT_Command_Port, TFT_RST_PIN)
#define TFT_RST_HIGH            GPIOWrite1(TFT_Command_Port, TFT_RST_PIN)

#define TFT_Data                (HWREG(TFT_Data_Port + (GPIO_O_DATA + (0xFF << 2))))

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
extern void PixelDraw(void *pvDisplayData, int32_t x, int32_t y, uint32_t color);
void LCD_Clear(uint16_t Color);

#endif /* SSD1289_H_ */
