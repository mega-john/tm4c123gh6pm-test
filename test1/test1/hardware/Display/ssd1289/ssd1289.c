/*
 * ssd1289.c
 *
 *  Created on: 24 нояб. 2014 г.
 *      Author: john
 */

#if  defined(SSD1289)

#include "ssd1289.h"

char BitSwap(char x)
{
 return ((x&0x01)<<8)|((x&0x02)<<6)|((x&0x04)<<4)|((x&0x08)<<2)|((x&0x10)>>2)|((x&20)>>4)|((x&0x40)>>6)|((x&0x80)>>8);
}

void TFT_Write_Bus(char VH, char VL)   //Параллельно данных Write функция
{
    GPIOPinWrite(TFT_Data_Port, 0xff, VH);
    TFT_WR_LOW;
    TFT_WR_HIGH;
    GPIOPinWrite(TFT_Data_Port, 0xff, VL);
    TFT_WR_LOW;
    TFT_WR_HIGH;
}

void TFT_Write_Data8(char VH, char VL) //8-Bit параметр для передачи данных
{
    TFT_RS_HIGH;
    TFT_Write_Bus(VH, VL);
}

void TFT_Write_Data16(int da)
{
    TFT_RS_HIGH;
    TFT_Write_Bus(da >> 8, da);
}

void TFT_Write_Command(int da)
{
    TFT_RS_LOW;
    TFT_Write_Bus(da >> 8, da);
}

void TFT_Write_Command_Data(int reg, int da)
{
    TFT_Write_Command(reg);
    TFT_Write_Data16(da);
    SysCtlDelay(30000);
}

void TFT_Write_Address(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    TFT_Write_Command_Data(0x0044, (x2 << 8) + x1);
    TFT_Write_Command_Data(0x0045, y1);
    TFT_Write_Command_Data(0x0046, y2);
    TFT_Write_Command_Data(0x004e, x1);
    TFT_Write_Command_Data(0x004f, y1);
    TFT_Write_Command(0x0022);
}

void PixelDraw(void *pvDisplayData, int32_t x, int32_t y, uint32_t color)
{
    TFT_CS_LOW;
    TFT_Write_Address(x, y, x, y);
    TFT_Write_Data16(color);
    TFT_CS_HIGH;
}

void LCD_Clear(uint16_t Color)
{
    uint8_t VH,VL;
    uint16_t i,j;
    VH=Color>>8;
    VL=Color;
    TFT_Write_Address(0,0,128-1,128-1);
    for(i=0;i<128;i++)
     {
      for (j=0;j<128;j++)
        {
          TFT_Write_Data8(VH,VL);
        }

      }
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
void UnlockPin1(uint32_t gpioPortBase, uint8_t pin)
{
    HWREG(gpioPortBase + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(gpioPortBase + GPIO_O_CR) = pin;
    HWREG(gpioPortBase + GPIO_O_LOCK) = 0;
}

PortFunctionInit(void)
{
    //
    // Enable Peripheral Clocks
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

//    UnlockPin1(GPIO_PORTB_BASE, GPIO_PIN_7);
    //
    // Enable pin PB7 for GPIOOutput
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_7);

    //
    // Enable pin PB6 for GPIOOutput
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_6);

    //
    // Enable pin PB4 for GPIOOutput
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4);

    //
    // Enable pin PB2 for GPIOOutput
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2);

    //
    // Enable pin PB5 for GPIOOutput
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5);

    //
    // Enable pin PB1 for GPIOOutput
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_1);

    //
    // Enable pin PB3 for GPIOOutput
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);

    //
    // Enable pin PB0 for GPIOOutput
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0);
}
void InitDisplay(void)
{
PortFunctionInit();
    SysCtlPeripheralEnable(TFT_Command_Periph);
    GPIOPinTypeGPIOOutput(TFT_Command_Port, TFT_RS_PIN|TFT_WR_PIN|TFT_CS_PIN|TFT_RST_PIN);
//
//    //start display initialization
//    TFT_RST_HIGH;
//    SysCtlDelay(30000);
//    TFT_RST_LOW;
//    SysCtlDelay(30000);
//    TFT_RST_HIGH;
//    SysCtlDelay(30000);
//    TFT_CS_LOW;
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x1);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x00);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x2);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x00);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x4);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x00);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x8);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x00);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x10);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x00);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x20);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x00);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x40);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x00);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x80);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x00);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0xff);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x00);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0xff);
//SysCtlDelay(30000);
//GPIOPinWrite(TFT_Data_Port, 0xff, 0x00);
//SysCtlDelay(30000);

//while(1);
    TFT_Write_Command_Data(0x0000, 0x0001);
//    delayMilliseconds(1);  //Открыть кристалл
    TFT_Write_Command_Data(0x0003, 0xA8A4);
//    delayMilliseconds(1);   //0xA8A4
    TFT_Write_Command_Data(0x000C, 0x0000);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x000D, 0x080C);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x000E, 0x2B00);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x001E, 0x00B7);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0001, 0x2B3F);
//    delayMilliseconds(1);   //Выход управления привода 320 * 240 0x6B3F
    TFT_Write_Command_Data(0x0002, 0x0600);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0010, 0x0000);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0011, 0x6070);
//    delayMilliseconds(1);        //0x4030           //Определить формат данных 16 цветов
    TFT_Write_Command_Data(0x0005, 0x0000);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0006, 0x0000);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0016, 0xEF1C);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0017, 0x0003);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0007, 0x0233);
//    delayMilliseconds(1);        //0x0233
    TFT_Write_Command_Data(0x000B, 0x0000);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x000F, 0x0000);
//    delayMilliseconds(1);        //Сканирование начальный адрес
    TFT_Write_Command_Data(0x0041, 0x0000);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0042, 0x0000);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0048, 0x0000);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0049, 0x013F);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x004A, 0x0000);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x004B, 0x0000);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0044, 0xEF00);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0045, 0x0000);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0046, 0x013F);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0030, 0x0707);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0031, 0x0204);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0032, 0x0204);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0033, 0x0502);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0034, 0x0507);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0035, 0x0204);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0036, 0x0204);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0037, 0x0502);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x003A, 0x0302);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x003B, 0x0302);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0023, 0x0000);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0024, 0x0000);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x0025, 0x8000);
//    delayMilliseconds(1);
    TFT_Write_Command_Data(0x004f, 0);        //ОК первый сайт 0
    TFT_Write_Command_Data(0x004e, 0);        //В первой колонке адрес 0
    TFT_Write_Command(0x0022);

//    TFT_CS_LOW;
}

const tDisplay psDisplay =
{ sizeof(tDisplay), 0, 240, 320, PixelDraw, PixelDrawMultiple, LineDrawH, LineDrawV, RectFill, ColorTranslate, Flush };

#endif
