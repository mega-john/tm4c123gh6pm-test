#include "global.h"

tCodePointMap g_psCodePointMap_language[] =
{
		{ CODEPAGE_WIN1251, CODEPAGE_UNICODE, GrMapWIN1251_Unicode },
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

tContext g_sContext;
uint8_t red_state, green_state, blue_state;

#define SW1 			GPIO_PIN_4
#define SW2 			GPIO_PIN_0
#define ALL_SWITCHES	(SW1 | SW2)
#define LED_RED 		GPIO_PIN_1
#define LED_BLUE 		GPIO_PIN_2
#define LED_GREEN 		GPIO_PIN_3
#define ALL_LEDS		(LED_RED | LED_BLUE | LED_GREEN)

void UnlockPin(uint32_t gpioPortBase, uint8_t pin)
{
	HWREG(gpioPortBase + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(gpioPortBase + GPIO_O_CR) = pin;//0xff;
	HWREG(gpioPortBase + GPIO_O_LOCK) = 0;
}

void UnlockPinF0()
{
	UnlockPin(GPIO_PORTF_BASE, GPIO_PIN_0);
//	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
//	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = GPIO_PIN_0;//0xff;
//	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
}

int main(void)
{
	FPULazyStackingEnable();
	SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_SYSDIV_2_5 | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	UnlockPinF0();
//	HWREGBITW()

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, ALL_LEDS);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, ALL_SWITCHES);
    GPIOPadConfigSet(GPIO_PORTF_BASE, ALL_SWITCHES, GPIO_STRENGTH_2MA,  GPIO_PIN_TYPE_STD_WPU);

	InitDisplay();

	GrContextInit(&g_sContext, &g_sILI9341);

	GrLibInit(&g_sGrLibDefaultlanguage);

	tRectangle sRect;
	sRect.i16XMin = 0;
	sRect.i16YMin = 0;
	sRect.i16XMax = DISPLAY_WIDTH;
	sRect.i16YMax = DISPLAY_HEIGHT;
	GrContextForegroundSet(&g_sContext, BACKGROUND);
	GrRectFill(&g_sContext, &sRect);

	GrContextForegroundSet(&g_sContext, FOREGROUND);
	GrRectDraw(&g_sContext, &sRect);

//	GrContextFontSet(&g_sContext, (tFont*) &g_sFontExArial36);
//	GrContextFontSet(&g_sContext, (tFont*) &g_sFontExArial24);
//	GrContextFontSet(&g_sContext, (tFont*) &g_sFontExComic36);
	GrContextFontSet(&g_sContext, (tFont*) &g_sFontExArial24);

	TFT_setOrientation(ORIENTATION_RIGHT2LEFT);

	GrTransparentImageDraw(&g_sContext, g_pui8ImageFuel, 10, 200, ClrWhite);

	MenuInitialize(&g_sContext);
	ButtonsInit();
	SetUpTimers();

//    GPIOPinWrite(GPIO_PORTF_BASE, ALL_LEDS, ALL_LEDS);

	while (1)
	{
		ProcessMenu();

        uint8_t ui8ButtonState;
        uint8_t ui8ButtonChanged;

        ui8ButtonState = ButtonsPoll(&ui8ButtonChanged, 0);

        if(BUTTON_PRESSED(DOWN_BUTTON, ui8ButtonState, ui8ButtonChanged))
    	{
    		MenuNavigate(MENU_NEXT);
    	}
    	else if(BUTTON_PRESSED(UP_BUTTON, ui8ButtonState, ui8ButtonChanged))
    	{
    		MenuNavigate(MENU_CHILD);
    		ClearScreen();
    	}
        GPIOPinWrite(GPIO_PORTF_BASE, ALL_LEDS, red_state | blue_state | green_state);
	}
}
