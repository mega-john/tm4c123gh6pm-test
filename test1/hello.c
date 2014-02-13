#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_hibernate.h"

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/fpu.h"
#include "grlib/grlib.h"

#include "hardware/buttons.h"
#include "hardware/ili9341/fonts/fonts.h"
#include "hardware/ili9341/ili9341.h"
//#include "hardware/ili9325/ili9325.h"
//#include "hardware/SPI/SPI.h"

tCodePointMap g_psCodePointMap_language[] =
{
//		{ CODEPAGE_WIN1251, CODEPAGE_UNICODE, GrMapWIN1251_Unicode },
		{ CODEPAGE_ISO8859_5, CODEPAGE_UNICODE, GrMapISO8859_5_Unicode},
};

#define NUM_CODEPOINT_MAPS (sizeof(g_psCodePointMap_language) / sizeof(tCodePointMap))

tGrLibDefaults g_sGrLibDefaultlanguage =
{
		GrDefaultStringRenderer,
		g_psCodePointMap_language,
		CODEPAGE_ISO8859_5,
		NUM_CODEPOINT_MAPS,
		0
};

int main(void)
{
	FPULazyStackingEnable();
	SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_SYSDIV_2_5 | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	InitDisplay();

	tContext g_sContext;
	GrContextInit(&g_sContext, &g_sILI9341);

	GrLibInit(&g_sGrLibDefaultlanguage);

	tRectangle sRect;
	sRect.i16XMin = 0;
	sRect.i16YMin = 0;
	sRect.i16XMax = 239;
	sRect.i16YMax = 319;
	GrContextForegroundSet(&g_sContext, ClrDarkBlue);
	GrRectFill(&g_sContext, &sRect);

	GrContextForegroundSet(&g_sContext, ClrWhite);
	GrRectDraw(&g_sContext, &sRect);

//    GrStringCodepageSet(&g_sContext, CODEPAGE_UTF_8);
	GrContextFontSet(&g_sContext, g_psFontWArial24pt);
	GrStringDraw(&g_sContext, "hello привет!", -1, 50, 50, 0);

	while (1)
	{
	}
}
