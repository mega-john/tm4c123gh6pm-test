/*
 * main.h
 *
 *  Created on: 05 θώνÿ 2014 γ.
 *      Author: estarcev
 */

#ifndef MAIN_H_
#define MAIN_H_

//#include <stdint.h>
#include "global.h"

tCodePointMap g_psCodePointMap_language[] =
{
        {CODEPAGE_WIN1251, CODEPAGE_UNICODE, GrMapWIN1251_Unicode}
};
#define NUM_CODEPOINT_MAPS (sizeof(g_psCodePointMap_language) / sizeof(tCodePointMap))

tGrLibDefaults g_sGrLibDefaultlanguage =
{
        GrDefaultStringRenderer,
        g_psCodePointMap_language,
        CODEPAGE_WIN1251,
        NUM_CODEPOINT_MAPS,
        0
};

#define FAST_GPIOPinWrite(ulPort, ucPins, ucVal) HWREG(ulPort + (GPIO_O_DATA + (ucPins << 2))) = ucVal


void SchedulerSysTickIntHandler(void);

#endif /* MAIN_H_ */
