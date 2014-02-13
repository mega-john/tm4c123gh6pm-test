//*****************************************************************************
//
// cfal96x64x16.c - Display driver for the Crystalfontz CFAL9664-F-B1 OLED
//                  display with an SSD1332.  This version uses an SSI
//                  interface to the display controller.
//
// Copyright (c) 2011-2013 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.0.1.11577 of the DK-TM4C123G Firmware Package.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "grlib/grlib.h"
#include "ili_initialization.h"
#include "ili9341.h"
#include "../SPI/SPI.h"

//*****************************************************************************
//
// Defines the SSI and GPIO peripherals that are used for this display.
//
//*****************************************************************************
#define DISPLAY_SSI_PERIPH          		SYSCTL_PERIPH_SSI3
#define DISPLAY_SSI_GPIO_PERIPH     	SYSCTL_PERIPH_GPIOD
#define DISPLAY_RST_GPIO_PERIPH     	SYSCTL_PERIPH_GPIOE

//*****************************************************************************
//
// Defines the GPIO pin configuration macros for the pins that are used for
// the SSI function.
//

#define DISPLAY_PINCFG_SSICLK       	GPIO_PD0_SSI3CLK//sck
#define DISPLAY_PINCFG_SSIFSS       	GPIO_PD1_SSI3FSS
#define DISPLAY_PINCFG_SSIRX        	GPIO_PD2_SSI3RX//miso
#define DISPLAY_PINCFG_SSITX        	GPIO_PD3_SSI3TX//mosi
//*****************************************************************************
//
// Defines the port and pins for the SSI peripheral.
//
//*****************************************************************************
#define DISPLAY_SSI_PORT            	GPIO_PORTD_BASE
#define DISPLAY_SSI_PINS            	(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)

//*****************************************************************************
//
// Defines the port and pins for the display voltage enable signal.
//
//*****************************************************************************
#define DISPLAY_CS_PORT            	GPIO_PORTE_BASE
#define DISPLAY_CS_PIN	             	GPIO_PIN_3

//*****************************************************************************
//
// Defines the port and pins for the display reset signal.
//
//*****************************************************************************
#define DISPLAY_RST_PORT            	GPIO_PORTE_BASE
#define DISPLAY_RST_PIN             	GPIO_PIN_2

//*****************************************************************************
//
// Defines the port and pins for the display Data/Command (D/C) signal.
//
//*****************************************************************************
#define DISPLAY_D_C_PORT            	GPIO_PORTE_BASE
#define DISPLAY_D_C_PIN             	GPIO_PIN_1

//*****************************************************************************
//
// Defines the SSI peripheral used and the data speed.
//
//*****************************************************************************
#define DISPLAY_SSI_BASE            	SSI2_BASE // SSI2
#define DISPLAY_SSI_CLOCK           	4000000

#define TFT_DC_LOW						GPIOPinWrite(DISPLAY_D_C_PORT, DISPLAY_D_C_PIN, 0)
#define TFT_CS_LOW						GPIOPinWrite(DISPLAY_CS_PORT, DISPLAY_CS_PIN, 0)
#define TFT_RST_ON						GPIOPinWrite(DISPLAY_RST_PORT, DISPLAY_RST_PIN, 0)
#define TFT_DC_HIGH						GPIOPinWrite(DISPLAY_D_C_PORT, DISPLAY_D_C_PIN, DISPLAY_D_C_PIN)
#define TFT_CS_HIGH						GPIOPinWrite(DISPLAY_CS_PORT, DISPLAY_CS_PIN, DISPLAY_CS_PIN)
#define TFT_RST_OFF						GPIOPinWrite(DISPLAY_RST_PORT, DISPLAY_RST_PIN, DISPLAY_RST_PIN)

//Basic Colors
#define TFT_RED             					0xf800
#define TFT_GREEN							0x07e0
#define TFT_BLUE								0x001f
#define TFT_BLACK							0x0000
#define TFT_YELLOW						0xffe0
#define TFT_WHITE							0xffff

//Other Colors
#define TFT_CYAN							0x07ff
#define TFT_PINK								0xf810
#define TFT_GRAY1							0x8410
#define TFT_GRAY2							0x4208
#define TFT_DARK_GREEN					0x02e0
#define TFT_MAGENTA						0xf8df

//TFT resolution 240*320
#define MIN_X									0
#define MIN_Y									0
#define MAX_X									239
#define MAX_Y									319
#define DELAY_VALUE						300000
#define constrain(amt, low, high) 		((amt) <= (low) ? (low) : ((amt) >= (high) ? (high) : (amt)))

uint8_t DisplayDirect = 0;

/**Macro definitions for char display direction**/
#define LEFT2RIGHT 0
#define DOWN2UP    1
#define RIGHT2LEFT 2
#define UP2DOWN    3

void TFT_sendCMD(uint8_t index)
{
	TFT_DC_LOW;
	TFT_CS_LOW;
	SPI_transfer8(index);
	TFT_CS_HIGH;
}

void TFT_WRITE_DATA(uint8_t data)
{
	TFT_DC_HIGH;
	TFT_CS_LOW;
	SPI_transfer8(data);
	TFT_CS_HIGH;
}

void TFT_sendData(uint16_t data)
{
	TFT_DC_HIGH;
	TFT_CS_LOW;
	SPI_transfer8(data >> 8);
	SPI_transfer8(data & 0xff);
	TFT_CS_HIGH;
}

uint8_t TFT_Read_Register(uint8_t Addr, uint8_t xParameter)
{
	uint8_t data = 0;
	TFT_sendCMD(0xd9); /* ext command                  */
	TFT_WRITE_DATA(0x10 + xParameter); /* 0x11 is the first Parameter  */
	TFT_DC_LOW; //digitalWrite(_dcPin,   LOW);
	TFT_CS_LOW; //cbi_macro(_port_cs, _bit_cs);
	SPI_transfer8(Addr);
	TFT_DC_HIGH; //digitalWrite(_dcPin,   HIGH);
	data = SPI_read8();
	TFT_CS_HIGH; //sbi_macro(_port_cs, _bit_cs);
	return data;
}

uint8_t TFT_readID(void)
{
	uint8_t i = 0;
	uint8_t data[3];
	uint8_t ID[3] =
	{ 0x00, 0x93, 0x41 };
	uint8_t ToF = 1;
	for (i = 0; i < 3; i++)
	{
		data[i] = TFT_Read_Register(0xd3, i + 1);
		if (data[i] != ID[i])
		{
			ToF = 0;
		}
	}
	if (!ToF) /* data!=ID                     */
	{
		//Serial.print("Read TFT ID failed, ID should be 0x09341, but read ID = 0x");
		for (i = 0; i < 3; i++)
		{
			//Serial.print(data[i],HEX);
		}
		//Serial.println();
	}
	return ToF;
}

void TFT_setCol(uint32_t StartCol, uint32_t EndCol)
{
	TFT_sendCMD(0x2A); //Column Address Set
	TFT_sendData(StartCol);
	TFT_sendData(EndCol);
}

void TFT_setPage(uint32_t StartPage, uint32_t EndPage)
{
	TFT_sendCMD(0x2B); //Page Address Set
	TFT_sendData(StartPage);
	TFT_sendData(EndPage);
}

void TFT_setXY(uint32_t poX, uint32_t poY)
{
	TFT_setCol(poX, poX);
	TFT_setPage(poY, poY);
	TFT_sendCMD(0x2c); //Memory Write
}

void PixelDraw(void *pvDisplayData, int32_t x, int32_t y, uint32_t color)
{
	TFT_setXY(x, y);
	TFT_sendData(color);
}

void LineDrawV(void *pvDisplayData, int32_t x, int32_t y1, int32_t y2,
		uint32_t color)
{
	int32_t length = y2 - y1;
	TFT_setCol(x, x);
	TFT_setPage(y1, y2);
	TFT_sendCMD(0x2c);
	TFT_DC_HIGH;
	TFT_CS_LOW;

	uint16_t i = 0;
	for (; i < length; i++)
	{
		SPI_transfer8(color >> 8);
		SPI_transfer8(color & 0xff);
	}

	TFT_CS_HIGH;
}

void LineDrawH(void *pvDisplayData, int32_t x1, int32_t x2, int32_t y,
		uint32_t color)
{
	int32_t length = x2 - x1;
	TFT_setCol(x1, x2);
	TFT_setPage(y, y);
	TFT_sendCMD(0x2c); //Memory Write

	TFT_DC_HIGH;
	TFT_CS_LOW;
	uint16_t i = 0;
	for (; i < length; i++)
	{
		SPI_transfer8(color >> 8);
		SPI_transfer8(color & 0xff);
	}

	TFT_CS_HIGH;
}

void TFT_set_orientation(uint8_t orientation)
{
	TFT_sendCMD(0x36); //wr_cmd(0x36);                     // MEMORY_ACCESS_CONTROL
	switch (orientation)
	{
	case 0:
		TFT_WRITE_DATA(0x48); //_spi.write(0x48);
		break;
	case 1:
		TFT_WRITE_DATA(0x28); //_spi.write(0x28);
		break;
	case 2:
		TFT_WRITE_DATA(0x88); //_spi.write(0x88);
		break;
	case 3:
		TFT_WRITE_DATA(0xE8); //_spi.write(0xE8);
		break;
	}
	//TFT_CS_HIGH;//_cs = 1;
}

uint32_t ColorTranslate(void *pvDisplayData, uint32_t ui32Value)
{
	return (COLOR24TO16BIT(ui32Value));
}

void Flush(void *pvDisplayData)
{
	//
	// There is nothing to be done.
	//
}

void RectFill(void *pvDisplayData, const tRectangle *pRect, uint32_t ui32Value)
{
	uint32_t uY;
	for (uY = pRect->i16YMin; uY <= pRect->i16YMax; uY++)
	{
		LineDrawH(0, pRect->i16XMin, pRect->i16XMax, uY, ui32Value);
	}
}

//*****************************************************************************
//
//! Draws a horizontal sequence of pixels on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param i32X is the X coordinate of the first pixel.
//! \param i32Y is the Y coordinate of the first pixel.
//! \param i32X0 is sub-pixel offset within the pixel data, which is valid for 1
//! or 4 bit per pixel formats.
//! \param i32Count is the number of pixels to draw.
//! \param i32BPP is the number of bits per pixel; must be 1, 4, or 8 optionally
//! ORed with various flags unused by this driver.
//! \param pui8Data is a pointer to the pixel data.  For 1 and 4 bit per pixel
//! formats, the most significant bit(s) represent the left-most pixel.
//! \param pui8Palette is a pointer to the palette used to draw the pixels.
//!
//! This function draws a horizontal sequence of pixels on the screen, using
//! the supplied palette.  For 1 bit per pixel format, the palette contains
//! pre-translated colors; for 4 and 8 bit per pixel formats, the palette
//! contains 24-bit RGB values that must be translated before being written to
//! the display.
//!
//! \return None.
//
//*****************************************************************************
void PixelDrawMultiple(void *pvDisplayData, int32_t x, int32_t y, int32_t x0,
		int32_t lCount, int32_t BPP, const uint8_t *pucData,
		const uint8_t *pucPalette)
{
	// Start talking to LCD
//		LCD_CS_ACTIVE

//	#ifdef LANDSCAPE
//		// Configure write direction to horizontal
//		LCDWriteCommand(ILI_ENTRY_MOD);
//		LCDWriteData(0x1038);
//	#endif

//		LCDGoto(lX,lY);
	TFT_setXY(x, y);

	uint32_t ulPixel = 0;
	uint32_t ulColor = 0;

	if (BPP == 1)
	{
		// 1 bit per pixel in pucData
		// lX0 is the index of the bit processed within a byte
		// pucPalette holds the pre-translated 32bit display color
		while (lCount)
		{
			ulPixel = *pucData++;

			while (lCount && x0 < 8)	// while there are pixels in this byte
			{
				ulColor = ((uint32_t *) pucPalette)[ulPixel & 1];// retrieve already translated color
				TFT_WRITE_DATA(ulColor);

				lCount--;		// processed another pixel
				x0++;			// done with this bit
				ulPixel >>= 1;	// prepare next bit
			}

			x0 = 0;	// process next byte, reset bit counter
		}
	}
	else if (BPP == 4)
	{
		// 4 bits per pixel in pucData
		// lX0 holds 0/1 to indicate 4-bit nibble within byte
		// pucPalette holds untranslated 24 bit color
		while (lCount)
		{
			if (x0 == 0)	// read first nibble
			{
				ulPixel = *pucData >> 4;
				x0 = 1;	// set index to second nibble
			}
			else
			{				// read second nibble
				ulPixel = *pucData & 0x0f;
				pucData++;// increase byte pointer as we're done reading this byte
				x0 = 0;	// set index to first nibble
			}

			ulColor = *(uint32_t *) (pucPalette + (ulPixel * 3)) & 0x00ffffff;// retrieve 24 bit color
			TFT_WRITE_DATA(COLOR24TO16BIT(ulColor));// translate and write to display

			lCount--;	// processed another pixel
		}
	}
	else if (BPP == 8)
	{
		// 8 bits per pixel in pucData
		// pucPalette holds untranslated 24 bit color
		while (lCount)
		{
			ulPixel = *pucData++;		// read pixel
			ulColor = *(uint32_t *) (pucPalette + (ulPixel * 3)) & 0x00ffffff;// retrieve 24 bit color
			TFT_WRITE_DATA(COLOR24TO16BIT(ulColor));// translate color and write to display
			lCount--;	// processed another pixel
		}
	}
	else if (BPP == 16)
	{
		// 16 bits per pixel
		// Pixel is in 16bit color, 5R 6G 5B format
		// No color translation needed for this display
		while (lCount)
		{
			ulPixel = *((unsigned short *) pucData);
			TFT_WRITE_DATA(ulPixel);
			pucData += 2;
			lCount--;
		}
	}

//	#ifdef LANDSCAPE
//		// Reset write direction to default (vertical)
//		LCDWriteCommand(ILI_ENTRY_MOD);
//		LCDWriteData(0x1030);
//	#endif

	// Done talking to LCD
//		LCD_CS_IDLE
}

void InitDisplay(void)
{
	SysCtlPeripheralEnable(DISPLAY_RST_GPIO_PERIPH);
	GPIOPinTypeGPIOOutput(DISPLAY_RST_PORT, DISPLAY_RST_PIN);
	GPIOPinTypeGPIOOutput(DISPLAY_CS_PORT, DISPLAY_CS_PIN);
	GPIOPinTypeGPIOOutput(DISPLAY_D_C_PORT, DISPLAY_D_C_PIN);

	//start display initialization
	SPI_begin(3);
	SPI_setDataMode(SPI_MODE3);
	/*
	 uint8_t ii = 0;
	 for(; ii < 11;ii++)
	 {
	 SPI_transfer(0);
	 }
	 */TFT_RST_ON;
	SysCtlDelay(DELAY_VALUE);
	TFT_RST_OFF;
	SysCtlDelay(DELAY_VALUE);

	uint8_t i = 0;
	uint8_t dSize = sizeof(init_data) / 2;
	for (i = 0; i < dSize; i++)
	{
		if (init_data[i][1] == 0)
		{
			TFT_sendCMD(init_data[i][0]);
		}
		else
		{
			TFT_WRITE_DATA(init_data[i][0]);
		}
	}
	SysCtlDelay(DELAY_VALUE);

	TFT_sendCMD(0x29);	        //Display on
	TFT_sendCMD(0x2c);	        //Memory Write
}

const tDisplay g_sILI9341 =
{ sizeof(tDisplay), 0, 240, 320, PixelDraw, PixelDrawMultiple, LineDrawH,
		LineDrawV, RectFill, ColorTranslate, Flush };
