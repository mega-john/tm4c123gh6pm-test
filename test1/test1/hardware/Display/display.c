/*
 * display.c
 *
 *  Created on: 24 но€б. 2014 г.
 *      Author: john
 */
#include "pictures/pictures.h"
#include "display.h"

tContext g_sContext;

//extern void InitDisplay(void);

void InitializeDisplay(void)
{

    SSD1289Init();
//    TFT_Clear(0x1234);

    GrContextInit(&g_sContext, &g_sSSD1289);

    GrLibInit(&g_sGrLibDefaultlanguage);

        tRectangle sRect;
        sRect.i16XMin = 0;
        sRect.i16YMin = 0;
        sRect.i16XMax = 240;
        sRect.i16YMax = 320;
        GrContextForegroundSet(&g_sContext, ClrDarkBlue);
        GrRectFill(&g_sContext, &sRect);

    GrTransparentImageDraw(&g_sContext, g_pui8FerrariCompressed, 0, 0, ClrWhite);

    while(1)
    {

    }


//    tRectangle sRect;
//    sRect.i16XMin = 0;
//    sRect.i16YMin = 0;
//    sRect.i16XMax = DISPLAY_WIDTH;
//    sRect.i16YMax = DISPLAY_HEIGHT;
//    GrContextForegroundSet(&g_sContext, BACKGROUND);
//    GrRectFill(&g_sContext, &sRect);
//
//    GrContextForegroundSet(&g_sContext, FOREGROUND);
//    GrRectDraw(&g_sContext, &sRect);
//
//    GrContextFontSet(&g_sContext, (tFont*) &g_sFontExArial24);
//
//    TFT_setOrientation(ORIENTATION_RIGHT2LEFT);
////    GrContextForegroundSet(&g_sContext, BACKGROUND);
////    GrContextBackgroundSet(&g_sContext, ClrYellow);
////    GrTransparentImageDraw(&g_sContext, g_pui8ImageFuelComp, 20, 200, ClrYellow);
//    GrContextForegroundSet(&g_sContext, FOREGROUND);
//    GrContextBackgroundSet(&g_sContext, BACKGROUND);
}
