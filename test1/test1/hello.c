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

tSchedulerTask g_psSchedulerTable[] =
{
//    { MeasureTemperature, 0, 1000, 0, true },
    { DrawTemperature, 0, 500, 0, true },
//    { DistanceIRTask, 0, 100, 0, true },
//    { AutoTask, 0, 10, 0, true },
};
uint32_t g_ui32SchedulerNumTasks = (sizeof(g_psSchedulerTable) / sizeof(tSchedulerTask));

tContext g_sContext;
uint8_t red_state, green_state, blue_state;

void UnlockPin(uint32_t gpioPortBase, uint8_t pin)
{
    HWREG(gpioPortBase + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(gpioPortBase + GPIO_O_CR) = pin;
    HWREG(gpioPortBase + GPIO_O_LOCK) = 0;
}

void UnlockPinF0()
{
    UnlockPin(GPIO_PORTF_BASE, GPIO_PIN_0);
}

void InitializePerepheral()
{
    FPULazyStackingEnable();
    SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_SYSDIV_2_5 | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    UnlockPinF0();

    ButtonsInit();
    SetUpTimers();

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

    GrContextFontSet(&g_sContext, (tFont*) &g_sFontExArial24);

    TFT_setOrientation(ORIENTATION_RIGHT2LEFT);
    GrContextForegroundSet(&g_sContext, BACKGROUND);
    GrContextBackgroundSet(&g_sContext, ClrYellow);
    GrTransparentImageDraw(&g_sContext, g_pui8ImageFuelComp, 20, 200, ClrYellow);
    GrContextForegroundSet(&g_sContext, FOREGROUND);
    GrContextBackgroundSet(&g_sContext, BACKGROUND);
    MenuInitialize(&g_sContext);

//    SearchTempSensors();
    SchedulerInit(1000);
}

int main(void)
{
    InitializePerepheral();

//    delay_ms(1000);

    while (1)
    {
        ProcessMenu();

        uint8_t ui8ButtonState;
        uint8_t ui8ButtonChanged;

        ui8ButtonState = ButtonsPoll(&ui8ButtonChanged, 0);

        if (BUTTON_PRESSED(DOWN_BUTTON, ui8ButtonState, ui8ButtonChanged))
        {
            MenuNavigate(MENU_NEXT);
        }
        else if (BUTTON_PRESSED(UP_BUTTON, ui8ButtonState, ui8ButtonChanged))
        {
            MenuNavigate(MENU_PREVIOUS);
        }
        else if (BUTTON_PRESSED(LEFT_BUTTON, ui8ButtonState, ui8ButtonChanged))
        {
            MenuNavigate(MENU_PARENT);
            ClearScreen();
        }
        else if (BUTTON_PRESSED(RIGHT_BUTTON, ui8ButtonState, ui8ButtonChanged))
        {
            MenuNavigate(MENU_CHILD);
            ClearScreen();
        }
        else if (BUTTON_PRESSED(SELECT_BUTTON, ui8ButtonState, ui8ButtonChanged))
        {
            MenuNavigate(MENU_CHILD);
            ClearScreen();
        }

//        if(cCount++ > 4000000)
//        {
//        	MeausureTemperature(0);
//        	cCount = 0;
//        }
        SchedulerRun();
    }
}
