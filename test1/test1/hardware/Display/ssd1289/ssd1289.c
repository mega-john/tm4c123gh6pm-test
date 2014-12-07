/*
 * ssd1289.c
 *
 *  Created on: 24 но€б. 2014 г.
 *      Author: john
 */

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"

#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "grlib/grlib.h"

#include "ssd1289.h"

#define TFT_WIDTH               239
#define TFT_HEIGHT              319

#define TFT_Data_Port           GPIO_PORTB_AHB_BASE
#define TFT_Data_Periph         SYSCTL_PERIPH_GPIOB

#define TFT_Command_Port        GPIO_PORTA_AHB_BASE//GPIO_PORTA_BASE
#define TFT_Command_Periph      SYSCTL_PERIPH_GPIOA
#define TFT_RD_PIN              GPIO_PIN_2//read, not not used-connect to power
#define TFT_RST_PIN             GPIO_PIN_3//reset
#define TFT_RS_PIN              GPIO_PIN_5//register select
#define TFT_WR_PIN              GPIO_PIN_6//write
#define TFT_CS_PIN              GPIO_PIN_7//chip select

#define GPIOWrite0(port, pin)   (HWREG(port + (pin << 2)) = 0)// GPIOPinWrite(port, pin, 0)
#define GPIOWrite1(port, pin)   (HWREG(port + (pin << 2)) = pin)//GPIOPinWrite(port, pin, pin)

#define TFT_RD_LOW              GPIOWrite0(TFT_Command_Port, TFT_RD_PIN)
#define TFT_RD_HIGH             GPIOWrite1(TFT_Command_Port, TFT_RD_PIN)
#define TFT_RST_LOW             GPIOWrite0(TFT_Command_Port, TFT_RST_PIN)
#define TFT_RST_HIGH            GPIOWrite1(TFT_Command_Port, TFT_RST_PIN)
#define TFT_CS_LOW              GPIOWrite0(TFT_Command_Port, TFT_CS_PIN)
#define TFT_CS_HIGH             GPIOWrite1(TFT_Command_Port, TFT_CS_PIN)
#define TFT_RS_LOW              GPIOWrite0(TFT_Command_Port, TFT_RS_PIN)
#define TFT_RS_HIGH             GPIOWrite1(TFT_Command_Port, TFT_RS_PIN)
#define TFT_WR_LOW              GPIOWrite0(TFT_Command_Port, TFT_WR_PIN)
#define TFT_WR_HIGH             GPIOWrite1(TFT_Command_Port, TFT_WR_PIN)

#define DPYCOLORTRANSLATE16(c)  ((((c) & 0x00f80000) >> 8) |                  \
                                 (((c) & 0x0000fc00) >> 5) |                  \
                                 (((c) & 0x000000f8) >> 3))
#define DPYCOLORTRANSLATE8(c)   ((((c) & 0x00e00000) >> 16) |                 \
                                 (((c) & 0x0000e000) >> 11) |                 \
                                 (((c) & 0x000000c0) >> 6))
#define DPYCOLORTRANSLATE DPYCOLORTRANSLATE8
// Macro to translate 24 bit RGB to 5-6-5 16 bit RGB
#define COLOR24TO16BIT(rgb)     ((((rgb) & 0x00f80000) >> 8) | (((rgb) & 0x0000fc00) >> 5) | (((rgb) & 0x000000f8) >> 3))

uint8_t BitSwap(uint8_t x)
{
    uint8_t res = 0;
    res =  ((x & 0x01) << 7) |
    ((x & 0x02) << 5) |
    ((x & 0x04) << 3) |
    ((x & 0x08) << 1) |
    ((x & 0x10) >> 1) |
    ((x & 0x20) >> 3) |
    ((x & 0x40) >> 5) |
    ((x & 0x80) >> 7);

    return res;
}

void TFT_WriteDataPortH1(char data)
{
    HWREG(GPIO_PORTF_AHB_BASE + ((GPIO_PIN_1) << 2)) = data >> 6;//msb
    HWREG(GPIO_PORTE_AHB_BASE + ((GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1) << 2)) = data >> 3;
    HWREG(GPIO_PORTD_AHB_BASE + ((GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0) << 2)) = data;//lsb
}

void TFT_WriteDataPortH(char data)
{
    HWREG(0x400593FC) = data;
}

void PortFunctionInit(void)
{
    //low byte periph
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_AHB_BASE, 0xff);

    //high byte periph
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_AHB_BASE, GPIO_PIN_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_AHB_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_AHB_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0);

    //command periph
    SysCtlPeripheralEnable(TFT_Command_Periph);
    SysCtlGPIOAHBEnable(TFT_Command_Periph);
    GPIOPinTypeGPIOOutput(TFT_Command_Port, TFT_RS_PIN | TFT_WR_PIN | TFT_CS_PIN | TFT_RST_PIN | TFT_RD_PIN);
}

void PortFunctionInit2(void)
{
    //low byte periph
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 0xff);

    //high byte periph
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

    //command periph
    SysCtlPeripheralEnable(TFT_Command_Periph);
    GPIOPinTypeGPIOOutput(TFT_Command_Port, TFT_RS_PIN | TFT_WR_PIN | TFT_CS_PIN | TFT_RST_PIN | TFT_RD_PIN);
}

void delayms(int count)  // /* X1ms */
{
    int i, j;
    for (i = 0; i < count; i++)
    {
        for (j = 0; j < 100; j++);
    }
}

//#define Bus16
void TFT_Write_Bus(int data)
{
#if defined(Bus16)
    TFT_WriteDataPortH(data >> 8);
    TFT_WriteDataPortL(data);
    TFT_WR_LOW;
    TFT_WR_HIGH;
#else
    TFT_WriteDataPortH(data >> 8);
    TFT_WR_LOW;
    TFT_WR_HIGH;
    TFT_WriteDataPortH(data);
    TFT_WR_LOW;
    TFT_WR_HIGH;
#endif
}

void TFT_Write_Command(int data)
{
    TFT_RS_LOW;
    TFT_CS_LOW;
    TFT_Write_Bus(data);
    TFT_CS_HIGH;
}

void TFT_Write_Data(int data)
{
    TFT_RS_HIGH;
    TFT_CS_LOW;
    TFT_Write_Bus(data);
    TFT_CS_HIGH;
}

void TFT_Write_Command_Data(int com1, int dat1)
{
    TFT_Write_Command(com1);
    TFT_Write_Data(dat1);
}

void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    TFT_Write_Command_Data(0x0044, (x2 << 8) + x1);
    TFT_Write_Command_Data(0x0045, y1);
    TFT_Write_Command_Data(0x0046, y2);
    TFT_Write_Command_Data(0x004e, x1);
    TFT_Write_Command_Data(0x004f, y1);
    TFT_Write_Command(0x0022);
}

void TFT_Clear(unsigned int color)
{
    int i, j;
    Address_set(0, 0, TFT_WIDTH, TFT_HEIGHT);

    TFT_RS_HIGH;
    TFT_CS_LOW;
    for (i = 0; i < 320; i++)
    {
        for (j = 0; j < 240; j++)
        {
//            TFT_Write_Data(color);
            TFT_Write_Bus(color);
       }
    }
    TFT_CS_HIGH;
}

void SSD1289Init(void)
{
    PortFunctionInit();

    TFT_RST_HIGH;
    delayms(5);
    TFT_RST_LOW;
    delayms(10);
    TFT_RST_HIGH;
    TFT_CS_HIGH;
    TFT_RD_HIGH;
    TFT_WR_HIGH;
    delayms(20);

    TFT_Write_Command_Data(0x0000,0x0001);
    TFT_Write_Command_Data(0x0003,0xA8A4);
    TFT_Write_Command_Data(0x000C,0x0000);
    TFT_Write_Command_Data(0x000D,0x080C);
    TFT_Write_Command_Data(0x000E,0x2B00);
    TFT_Write_Command_Data(0x001E,0x00B7);
    TFT_Write_Command_Data(0x0001,0x2B3F);
    TFT_Write_Command_Data(0x0002,0x0600);
    TFT_Write_Command_Data(0x0010,0x0000);
    TFT_Write_Command_Data(0x0011,0x6070);
    TFT_Write_Command_Data(0x0005,0x0000);
    TFT_Write_Command_Data(0x0006,0x0000);
    TFT_Write_Command_Data(0x0016,0xEF1C);
    TFT_Write_Command_Data(0x0017,0x0003);
    TFT_Write_Command_Data(0x0007,0x0233);
    TFT_Write_Command_Data(0x000B,0x0000);
    TFT_Write_Command_Data(0x000F,0x0000);
    TFT_Write_Command_Data(0x0041,0x0000);
    TFT_Write_Command_Data(0x0042,0x0000);
    TFT_Write_Command_Data(0x0048,0x0000);
    TFT_Write_Command_Data(0x0049,0x013F);
    TFT_Write_Command_Data(0x004A,0x0000);
    TFT_Write_Command_Data(0x004B,0x0000);
    TFT_Write_Command_Data(0x0044,0xEF00);
    TFT_Write_Command_Data(0x0045,0x0000);
    TFT_Write_Command_Data(0x0046,0x013F);
    TFT_Write_Command_Data(0x0030,0x0707);
    TFT_Write_Command_Data(0x0031,0x0204);
    TFT_Write_Command_Data(0x0032,0x0204);
    TFT_Write_Command_Data(0x0033,0x0502);
    TFT_Write_Command_Data(0x0034,0x0507);
    TFT_Write_Command_Data(0x0035,0x0204);
    TFT_Write_Command_Data(0x0036,0x0204);
    TFT_Write_Command_Data(0x0037,0x0502);
    TFT_Write_Command_Data(0x003A,0x0302);
    TFT_Write_Command_Data(0x003B,0x0302);
    TFT_Write_Command_Data(0x0023,0x0000);
    TFT_Write_Command_Data(0x0024,0x0000);
    TFT_Write_Command_Data(0x0025,0x8000);
    TFT_Write_Command_Data(0x004f,0);
    TFT_Write_Command_Data(0x004e,0);
    TFT_Write_Command(0x0022);
}

void PixelDraw(void *pvDisplayData, int32_t x, int32_t y, uint32_t color)
{
    Address_set(x, y, x, y);
    TFT_Write_Data((uint16_t)(color & 0x0000ffff));
}

void PixelDrawMultiple(void *pvDisplayData, int32_t x, int32_t y, int32_t x0, int32_t lCount, int32_t BPP, const uint8_t *pucData,
        const uint8_t *pucPalette)
{
    Address_set(x, y, TFT_WIDTH, TFT_HEIGHT);

    TFT_RS_HIGH;
    TFT_CS_LOW;

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

            while (lCount && x0 < 8)    // while there are pixels in this byte
            {
                ulColor = ((uint32_t *) pucPalette)[ulPixel & 1];    // retrieve already translated color
                TFT_Write_Bus(ulColor);

                lCount--;       // processed another pixel
                x0++;           // done with this bit
                ulPixel >>= 1;  // prepare next bit
            }

            x0 = 0; // process next byte, reset bit counter
        }
    }
    else if (BPP == 4)
    {
        // 4 bits per pixel in pucData
        // lX0 holds 0/1 to indicate 4-bit nibble within byte
        // pucPalette holds untranslated 24 bit color
        while (lCount)
        {
            if (x0 == 0)    // read first nibble
            {
                ulPixel = *pucData >> 4;
                x0 = 1; // set index to second nibble
            }
            else
            {               // read second nibble
                ulPixel = *pucData & 0x0f;
                pucData++;               // increase byte pointer as we're done reading this byte
                x0 = 0; // set index to first nibble
            }

            ulColor = *(uint32_t *) (pucPalette + (ulPixel * 3)) & 0x00ffffff; // retrieve 24 bit color
            TFT_Write_Bus(COLOR24TO16BIT(ulColor));

            lCount--;   // processed another pixel
        }
    }
    else if (BPP == 8)
    {
        // 8 bits per pixel in pucData
        // pucPalette holds untranslated 24 bit color
        while (lCount)
        {
            ulPixel = *pucData++;       // read pixel
            ulColor = *(uint32_t *) (pucPalette + (ulPixel * 3)) & 0x00ffffff;       // retrieve 24 bit color
            TFT_Write_Bus(COLOR24TO16BIT(ulColor));
           lCount--;   // processed another pixel
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
            TFT_Write_Bus(ulPixel);
            pucData += 2;
            lCount--;
        }
    }

    TFT_CS_HIGH;
}

void LineDrawV(void *pvDisplayData, int32_t x, int32_t y1, int32_t y2, uint32_t color)
{
    Address_set(x, y1, x, y2);
    uint32_t i = y1;
    TFT_RS_HIGH;
    TFT_CS_LOW;
    for(; i <= y2; i++)
    {
        TFT_Write_Bus(color);
    }
    TFT_CS_HIGH;
}

void LineDrawH(void *pvDisplayData, int32_t x1, int32_t x2, int32_t y, uint32_t color)
{
    Address_set(x1, y, x2, y);
    uint32_t i = x1;
    TFT_RS_HIGH;
    TFT_CS_LOW;
    for(; i <= x2; i++)
    {
        TFT_Write_Bus(color);
    }
    TFT_CS_HIGH;
}

uint32_t ColorTranslate(void *pvDisplayData, uint32_t ui32Value)
{
//    return 0;
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

const tDisplay g_sSSD1289 =
{ sizeof(tDisplay), 0, 240, 320, PixelDraw, PixelDrawMultiple, LineDrawH, LineDrawV, RectFill, ColorTranslate, Flush };

