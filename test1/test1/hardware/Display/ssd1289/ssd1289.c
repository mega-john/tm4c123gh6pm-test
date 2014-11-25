/*
 * ssd1289.c
 *
 *  Created on: 24 нояб. 2014 г.
 *      Author: john
 */

#if  defined(SSD1289)

#include "ssd1289.h"

void LCD_Writ_Bus(char VH, char VL)   //Параллельно данных Write функция
{
    LCD_DataPortH = VH;
    LCD_WR = 0;
    LCD_WR = 1;
    LCD_DataPortH = VL;
    LCD_WR = 0;
    LCD_WR = 1;
}

void LCD_WR_DATA8(char VH, char VL) //8-Bit параметр для передачи данных
{
    LCD_RS = 1;
    LCD_Writ_Bus(VH, VL);
}

void LCD_WR_DATA(int da)
{
    LCD_RS = 1;
    LCD_Writ_Bus(da >> 8, da);
}

void LCD_WR_REG(int da)
{
    LCD_RS = 0;
    LCD_Writ_Bus(da >> 8, da);
}

void LCD_WR_REG_DATA(int reg, int da)
{
    LCD_WR_REG(reg);
    LCD_WR_DATA(da);
}

void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    LCD_WR_REG_DATA(0x0044, (x2 << 8) + x1);
    LCD_WR_REG_DATA(0x0045, y1);
    LCD_WR_REG_DATA(0x0046, y2);
    LCD_WR_REG_DATA(0x004e, x1);
    LCD_WR_REG_DATA(0x004f, y1);
    LCD_WR_REG(0x0022);
}

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
    LCD_WR_REG_DATA(0x0000, 0x0001);
    delayms(1);  //Открыть кристалл
    LCD_WR_REG_DATA(0x0003, 0xA8A4);
    delayms(1);   //0xA8A4
    LCD_WR_REG_DATA(0x000C, 0x0000);
    delayms(1);
    LCD_WR_REG_DATA(0x000D, 0x080C);
    delayms(1);
    LCD_WR_REG_DATA(0x000E, 0x2B00);
    delayms(1);
    LCD_WR_REG_DATA(0x001E, 0x00B7);
    delayms(1);
    LCD_WR_REG_DATA(0x0001, 0x2B3F);
    delayms(1);   //Выход управления привода 320 * 240 0x6B3F
    LCD_WR_REG_DATA(0x0002, 0x0600);
    delayms(1);
    LCD_WR_REG_DATA(0x0010, 0x0000);
    delayms(1);
    LCD_WR_REG_DATA(0x0011, 0x6070);
    delayms(1);        //0x4030           //Определить формат данных 16 цветов
    LCD_WR_REG_DATA(0x0005, 0x0000);
    delayms(1);
    LCD_WR_REG_DATA(0x0006, 0x0000);
    delayms(1);
    LCD_WR_REG_DATA(0x0016, 0xEF1C);
    delayms(1);
    LCD_WR_REG_DATA(0x0017, 0x0003);
    delayms(1);
    LCD_WR_REG_DATA(0x0007, 0x0233);
    delayms(1);        //0x0233
    LCD_WR_REG_DATA(0x000B, 0x0000);
    delayms(1);
    LCD_WR_REG_DATA(0x000F, 0x0000);
    delayms(1);        //Сканирование начальный адрес
    LCD_WR_REG_DATA(0x0041, 0x0000);
    delayms(1);
    LCD_WR_REG_DATA(0x0042, 0x0000);
    delayms(1);
    LCD_WR_REG_DATA(0x0048, 0x0000);
    delayms(1);
    LCD_WR_REG_DATA(0x0049, 0x013F);
    delayms(1);
    LCD_WR_REG_DATA(0x004A, 0x0000);
    delayms(1);
    LCD_WR_REG_DATA(0x004B, 0x0000);
    delayms(1);
    LCD_WR_REG_DATA(0x0044, 0xEF00);
    delayms(1);
    LCD_WR_REG_DATA(0x0045, 0x0000);
    delayms(1);
    LCD_WR_REG_DATA(0x0046, 0x013F);
    delayms(1);
    LCD_WR_REG_DATA(0x0030, 0x0707);
    delayms(1);
    LCD_WR_REG_DATA(0x0031, 0x0204);
    delayms(1);
    LCD_WR_REG_DATA(0x0032, 0x0204);
    delayms(1);
    LCD_WR_REG_DATA(0x0033, 0x0502);
    delayms(1);
    LCD_WR_REG_DATA(0x0034, 0x0507);
    delayms(1);
    LCD_WR_REG_DATA(0x0035, 0x0204);
    delayms(1);
    LCD_WR_REG_DATA(0x0036, 0x0204);
    delayms(1);
    LCD_WR_REG_DATA(0x0037, 0x0502);
    delayms(1);
    LCD_WR_REG_DATA(0x003A, 0x0302);
    delayms(1);
    LCD_WR_REG_DATA(0x003B, 0x0302);
    delayms(1);
    LCD_WR_REG_DATA(0x0023, 0x0000);
    delayms(1);
    LCD_WR_REG_DATA(0x0024, 0x0000);
    delayms(1);
    LCD_WR_REG_DATA(0x0025, 0x8000);
    delayms(1);
    LCD_WR_REG_DATA(0x004f, 0);        //ОК первый сайт 0
    LCD_WR_REG_DATA(0x004e, 0);        //В первой колонке адрес 0
    LCD_WR_REG(0x0022);

}

const tDisplay psDisplay =
{ sizeof(tDisplay), 0, 240, 320, PixelDraw, PixelDrawMultiple, LineDrawH, LineDrawV, RectFill, ColorTranslate, Flush };

#endif
