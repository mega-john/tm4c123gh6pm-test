/*
 * ssd1289.c
 *
 *  Created on: 24 ����. 2014 �.
 *      Author: john
 */

#include "ssd1289.h"

#ifndef ILI9341

void PixelDraw(void *pvDisplayData, int32_t x, int32_t y, uint32_t color)
{
//    TFT_setXY(x, y);
//    TFT_sendData16(color);
}

void PixelDrawMultiple(void *pvDisplayData, int32_t x, int32_t y, int32_t x0, int32_t lCount, int32_t BPP, const uint8_t *pucData,
        const uint8_t *pucPalette)
{
//    TFT_setCol(x, DISPLAY_WIDTH);
//    TFT_setPage(y, DISPLAY_HEIGHT);
//    TFT_sendCMD(0x2c);
//
//    uint32_t ulPixel = 0;
//    uint32_t ulColor = 0;
//
//    if (BPP == 1)
//    {
//        // 1 bit per pixel in pucData
//        // lX0 is the index of the bit processed within a byte
//        // pucPalette holds the pre-translated 32bit display color
//        while (lCount)
//        {
//            ulPixel = *pucData++;
//
//            while (lCount && x0 < 8)    // while there are pixels in this byte
//            {
//                ulColor = ((uint32_t *) pucPalette)[ulPixel & 1];    // retrieve already translated color
//                TFT_sendData16(ulColor);
//
//                lCount--;       // processed another pixel
//                x0++;           // done with this bit
//                ulPixel >>= 1;  // prepare next bit
//            }
//
//            x0 = 0; // process next byte, reset bit counter
//        }
//    }
//    else if (BPP == 4)
//    {
//        // 4 bits per pixel in pucData
//        // lX0 holds 0/1 to indicate 4-bit nibble within byte
//        // pucPalette holds untranslated 24 bit color
//        while (lCount)
//        {
//            if (x0 == 0)    // read first nibble
//            {
//                ulPixel = *pucData >> 4;
//                x0 = 1; // set index to second nibble
//            }
//            else
//            {               // read second nibble
//                ulPixel = *pucData & 0x0f;
//                pucData++;               // increase byte pointer as we're done reading this byte
//                x0 = 0; // set index to first nibble
//            }
//
//            ulColor = *(uint32_t *) (pucPalette + (ulPixel * 3)) & 0x00ffffff; // retrieve 24 bit color
//            TFT_sendData16(COLOR24TO16BIT(ulColor)); // translate and write to display
//
//            lCount--;   // processed another pixel
//        }
//    }
//    else if (BPP == 8)
//    {
//        // 8 bits per pixel in pucData
//        // pucPalette holds untranslated 24 bit color
//        while (lCount)
//        {
//            ulPixel = *pucData++;       // read pixel
//            ulColor = *(uint32_t *) (pucPalette + (ulPixel * 3)) & 0x00ffffff;       // retrieve 24 bit color
//            TFT_sendData16(COLOR24TO16BIT(ulColor));       // translate and write to display
//            lCount--;   // processed another pixel
//        }
//    }
//    else if (BPP == 16)
//    {
//        // 16 bits per pixel
//        // Pixel is in 16bit color, 5R 6G 5B format
//        // No color translation needed for this display
//        while (lCount)
//        {
//            ulPixel = *((uint16_t *) pucData);
//            TFT_sendData16(ulPixel);
//            pucData += 2;
//            lCount--;
//        }
//    }
}

void LineDrawV(void *pvDisplayData, int32_t x, int32_t y1, int32_t y2, uint32_t color)
{
//    int32_t length = y2 - y1;
//    TFT_setCol(x, x);
//    TFT_setPage(y1, y2);
//    TFT_sendCMD(0x2c);
//    TFT_DC_HIGH;
//    TFT_CS_LOW;
//
//    int32_t i = 0;
//    for (; i < length; i++)
//    {
//        SPI_transfer8(color >> 8);
//        SPI_transfer8(color & 0xff);
//    }
//
//    TFT_CS_HIGH;
}

void LineDrawH(void *pvDisplayData, int32_t x1, int32_t x2, int32_t y, uint32_t color)
{
//    int32_t length = x2 - x1;
//    TFT_setCol(x1, x2);
//    TFT_setPage(y, y);
//    TFT_sendCMD(0x2c);
//
//    TFT_DC_HIGH;
//    TFT_CS_LOW;
//    int32_t i = 0;
//    for (; i < length; i++)
//    {
//        SPI_transfer8(color >> 8);
//        SPI_transfer8(color & 0xff);
//    }
//
//    TFT_CS_HIGH;
}

uint32_t ColorTranslate(void *pvDisplayData, uint32_t ui32Value)
{
    return 0;
//    return (COLOR24TO16BIT(ui32Value));
}

void Flush(void *pvDisplayData)
{
    //
    // There is nothing to be done.
    //
}

void RectFill(void *pvDisplayData, const tRectangle *pRect, uint32_t ui32Value)
{
//    uint32_t uY;
//    for (uY = pRect->i16YMin; uY <= pRect->i16YMax; uY++)
//    {
//        LineDrawH(0, pRect->i16XMin, pRect->i16XMax, uY, ui32Value);
//    }
}

void InitDisplay(void)
{
//    SysCtlPeripheralEnable(DISPLAY_RST_PERIPH);
//    GPIOPinTypeGPIOOutput(DISPLAY_RST_PORT, DISPLAY_RST_PIN);
//    SysCtlPeripheralEnable(DISPLAY_CS_PERIPH);
//    GPIOPinTypeGPIOOutput(DISPLAY_CS_PORT, DISPLAY_CS_PIN);
//    SysCtlPeripheralEnable(DISPLAY_D_C_PERIPH);
//    GPIOPinTypeGPIOOutput(DISPLAY_D_C_PORT, DISPLAY_D_C_PIN);
//
//    //start display initialization
//    SPI_begin(3);
//    SPI_setDataMode(SPI_MODE3);
//    TFT_RST_ON;
//    SysCtlDelay(DELAY_VALUE);
//    TFT_RST_OFF;
//    SysCtlDelay(DELAY_VALUE);
//
//    uint16_t i = 0;
//    uint16_t dSize = sizeof(init_data) / 2;
//    for (; i < dSize; i++)
//    {
//        if (init_data[i][1] == 0)
//        {
//            TFT_sendCMD(init_data[i][0]);
//        }
//        else
//        {
//            TFT_sendData8(init_data[i][0]);
//        }
//    }
//    SysCtlDelay(DELAY_VALUE);
//
//    TFT_sendCMD(0x29);          //Display on
//    TFT_sendCMD(0x2c);          //Memory Write
}

const tDisplay psDisplay =
{
        sizeof(tDisplay),
        0,
        240,
        320,
        PixelDraw,
        PixelDrawMultiple,
        LineDrawH,
        LineDrawV,
        RectFill,
        ColorTranslate,
        Flush
};

#endif
