/*
 * display.c
 *
 *  Created on: 24 но€б. 2014 г.
 *      Author: john
 */

#include "display.h"

tContext g_sContext;

extern void InitDisplay(void);

void InitializeDisplay(void)
{
    InitDisplay();

    GrContextInit(&g_sContext, &psDisplay);

    GrLibInit(&g_sGrLibDefaultlanguage);

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
