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
#define DISPLAY_SSI_PERIPH          	SYSCTL_PERIPH_SSI3
#define DISPLAY_SSI_GPIO_PERIPH     	SYSCTL_PERIPH_GPIOD
#define DISPLAY_RST_GPIO_PERIPH     	SYSCTL_PERIPH_GPIOE

//*****************************************************************************
//
// Defines the GPIO pin configuration macros for the pins that are used for
// the SSI function.
//
//*****************************************************************************
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
// Defines the port and pins for the display C/S signal.
//
//*****************************************************************************
#define DISPLAY_CS_PORT            		GPIO_PORTE_BASE
#define DISPLAY_CS_PIN	             	GPIO_PIN_3
#define TFT_CS_LOW						GPIOPinWrite(DISPLAY_CS_PORT, DISPLAY_CS_PIN, 0)
#define TFT_CS_HIGH						GPIOPinWrite(DISPLAY_CS_PORT, DISPLAY_CS_PIN, DISPLAY_CS_PIN)

//*****************************************************************************
//
// Defines the port and pins for the display reset signal.
//
//*****************************************************************************
#define DISPLAY_RST_PORT            	GPIO_PORTE_BASE
#define DISPLAY_RST_PIN             	GPIO_PIN_2
#define TFT_RST_ON						GPIOPinWrite(DISPLAY_RST_PORT, DISPLAY_RST_PIN, 0)
#define TFT_RST_OFF						GPIOPinWrite(DISPLAY_RST_PORT, DISPLAY_RST_PIN, DISPLAY_RST_PIN)

//*****************************************************************************
//
// Defines the port and pins for the display Data/Command (D/C) signal.
//
//*****************************************************************************
#define DISPLAY_D_C_PORT            	GPIO_PORTE_BASE
#define DISPLAY_D_C_PIN             	GPIO_PIN_1
#define TFT_DC_LOW						GPIOPinWrite(DISPLAY_D_C_PORT, DISPLAY_D_C_PIN, 0)
#define TFT_DC_HIGH						GPIOPinWrite(DISPLAY_D_C_PORT, DISPLAY_D_C_PIN, DISPLAY_D_C_PIN)

#define DELAY_VALUE						300000
#define constrain(amt, low, high) 		((amt) <= (low) ? (low) : ((amt) >= (high) ? (high) : (amt)))

void TFT_sendCMD(uint8_t index)
{
	TFT_DC_LOW;
	TFT_CS_LOW;
	SPI_transfer8(index);
	TFT_CS_HIGH;
}

void TFT_sendData8(uint8_t data)
{
	TFT_DC_HIGH;
	TFT_CS_LOW;
	SPI_transfer8(data);
	TFT_CS_HIGH;
}

void TFT_sendData16(uint16_t data)
{
	TFT_DC_HIGH;
	TFT_CS_LOW;
	SPI_transfer16(data);
	TFT_CS_HIGH;
}

void TFT_sendData24(uint32_t data)
{
	TFT_DC_HIGH;
	TFT_CS_LOW;
	SPI_transfer24(data);
	TFT_CS_HIGH;
}

void TFT_sendData32(uint32_t data)
{
	TFT_DC_HIGH;
	TFT_CS_LOW;
	SPI_transfer32(data);
	TFT_CS_HIGH;
}

uint8_t TFT_Read_Register(uint8_t Addr, uint8_t xParameter)
{
	uint8_t data = 0;
	TFT_sendCMD(0xd9); /* ext command                  */
	TFT_sendData8(0x10 + xParameter); /* 0x11 is the first Parameter  */
	TFT_DC_LOW;
	TFT_CS_LOW;
	SPI_transfer8(Addr);
	TFT_DC_HIGH;
	data = SPI_read8();
	TFT_CS_HIGH;
	return data;
}

uint8_t TFT_readID(void)
{
	uint8_t i = 0;
	uint8_t data[3];
	uint8_t ID[3] = { 0x00, 0x93, 0x41 };
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
	TFT_sendData16(StartCol);
	TFT_sendData16(EndCol);
}

void TFT_setPage(uint32_t StartPage, uint32_t EndPage)
{
	TFT_sendCMD(0x2B); //Page Address Set
	TFT_sendData16(StartPage);
	TFT_sendData16(EndPage);
}

void TFT_setXY(uint32_t poX, uint32_t poY)
{
	TFT_setCol(poX, poX);
	TFT_setPage(poY, poY);
	TFT_sendCMD(0x2c); //Memory Write
}

void TFT_setOrientation(uint8_t orientation)
{
	TFT_sendCMD(0x36);
	switch (orientation)
	{
	case 0:
		TFT_sendData8(0x48);
		break;
	case 1:
		TFT_sendData8(0x28);
		break;
	case 2:
		TFT_sendData8(0x88);
		break;
	case 3:
		TFT_sendData8(0xE8);
		break;
	}
}

void PixelDraw(void *pvDisplayData, int32_t x, int32_t y, uint32_t color)
{
	TFT_setXY(x, y);
	TFT_sendData16(color);
}

void PixelDrawMultiple(void *pvDisplayData,
						int32_t x,
						int32_t y,
						int32_t x0,
						int32_t lCount,
						int32_t BPP,
						const uint8_t *pucData,
						const uint8_t *pucPalette)
{
	TFT_setCol(x, DISPLAY_WIDTH);
	TFT_setPage(y, DISPLAY_HEIGHT);
	TFT_sendCMD(0x2c);

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
				TFT_sendData16(ulColor);

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
			TFT_sendData16(COLOR24TO16BIT(ulColor));// translate and write to display

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
			TFT_sendData16(COLOR24TO16BIT(ulColor));// translate and write to display
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
			ulPixel = *((uint16_t *) pucData);
			TFT_sendData16(ulPixel);
			pucData += 2;
			lCount--;
		}
	}
}

void LineDrawV(void *pvDisplayData, int32_t x, int32_t y1, int32_t y2, uint32_t color)
{
	int32_t length = y2 - y1;
	TFT_setCol(x, x);
	TFT_setPage(y1, y2);
	TFT_sendCMD(0x2c);
	TFT_DC_HIGH;
	TFT_CS_LOW;

	int32_t i = 0;
	for (; i < length; i++)
	{
		SPI_transfer8(color >> 8);
		SPI_transfer8(color & 0xff);
	}

	TFT_CS_HIGH;
}

void LineDrawH(void *pvDisplayData, int32_t x1, int32_t x2, int32_t y, uint32_t color)
{
	int32_t length = x2 - x1;
	TFT_setCol(x1, x2);
	TFT_setPage(y, y);
	TFT_sendCMD(0x2c);

	TFT_DC_HIGH;
	TFT_CS_LOW;
	int32_t i = 0;
	for (; i < length; i++)
	{
		SPI_transfer8(color >> 8);
		SPI_transfer8(color & 0xff);
	}

	TFT_CS_HIGH;
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
	 */
	TFT_RST_ON;
	SysCtlDelay(DELAY_VALUE);
	TFT_RST_OFF;
	SysCtlDelay(DELAY_VALUE);

	uint16_t i = 0;
	uint16_t dSize = sizeof(init_data) / 2;
	for (; i < dSize; i++)
	{
		if (init_data[i][1] == 0)
		{
			TFT_sendCMD(init_data[i][0]);
		}
		else
		{
			TFT_sendData8(init_data[i][0]);
		}
	}
	SysCtlDelay(DELAY_VALUE);

	TFT_sendCMD(0x29);	        //Display on
	TFT_sendCMD(0x2c);	        //Memory Write
}

const tDisplay g_sILI9341 =
{
		sizeof(tDisplay),
		0,
		DISPLAY_WIDTH,
		DISPLAY_HEIGHT,
		PixelDraw,
		PixelDrawMultiple,
		LineDrawH,
		LineDrawV,
		RectFill,
		ColorTranslate,
		Flush
};
