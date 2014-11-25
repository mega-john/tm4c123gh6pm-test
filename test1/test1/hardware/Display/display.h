/*
 * display.h
 *
 *  Created on: 24 но€б. 2014 г.
 *      Author: john
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "../../global.h"

static tCodePointMap g_psCodePointMap_language[] =
{
        {CODEPAGE_WIN1251, CODEPAGE_UNICODE, GrMapWIN1251_Unicode}
};
#define NUM_CODEPOINT_MAPS (sizeof(g_psCodePointMap_language) / sizeof(tCodePointMap))

static tGrLibDefaults g_sGrLibDefaultlanguage =
{
        GrDefaultStringRenderer,
        g_psCodePointMap_language,
        CODEPAGE_WIN1251,
        NUM_CODEPOINT_MAPS,
        0
};

#if  defined(ILI9341)
    #include "ili9341/ili9341.h"
#elif defined(SSD1289)
    #include "ssd1289/ssd1289.h"
#else
    #error Define diplay type!
#endif

void InitializeDisplay(void);

#endif /* DISPLAY_H_ */
