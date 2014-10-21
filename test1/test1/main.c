#include "main.h"

volatile update_flags flags;

void __error__(char *pcFilename, uint32_t ui32Line)
{
    UARTprintf("\r!__error__! file: %s line: %i", pcFilename, ui32Line);
}

void ShakePin(void * params)
{
//    static bool isSet = false;
//
//    if (isSet)
//    {
//        FAST_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0);
////        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0);
//        isSet = false;
//    }
//    else
//    {
//        FAST_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6);
////        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6);
//        isSet = true;
//    }
}

//tSchedulerTask g_psSchedulerTable[] =
//{
////    { MeasureTemperature, 0, 1000, 0, true },
//    { DrawTemperature, 0, 500, 0, true },
////    { DistanceIRTask, 0, 100, 0, true },
////    { ShakePin, 0, 2, 0, true },
//};
//uint32_t g_ui32SchedulerNumTasks = (sizeof(g_psSchedulerTable) / sizeof(tSchedulerTask));

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

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

//#define INTERNAL_EEPROM_SAVE_ADDRESS    0x3FF00

extern float Distance;

void SetupSystemClock()
{
    //
    //  Run at system clock at 80MHz
    //
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
}

void InitializePerepheral()
{
    SetupSystemClock();

    FPULazyStackingEnable();

    DelayInit();

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    IntMasterDisable();

    UnlockPinF0();

//    ButtonsInit();
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
//    GrContextForegroundSet(&g_sContext, BACKGROUND);
//    GrContextBackgroundSet(&g_sContext, ClrYellow);
//    GrTransparentImageDraw(&g_sContext, g_pui8ImageFuelComp, 20, 200, ClrYellow);
    GrContextForegroundSet(&g_sContext, FOREGROUND);
    GrContextBackgroundSet(&g_sContext, BACKGROUND);
    MenuInitialize(&g_sContext);

    InitI2C();

    ConfigureUART();

//    SetupExternalInterrupts();

    IntMasterEnable();
}

void TestEEPROM()
{
	uint32_t res = 0;
//    uint8_t write_buf[6] = {0xa1, 0xb2, 0xc3, 0xd4, 0xe5, 0xf6};
//    uint16_t l = sizeof(g_pui8ImageFuelComp);
	UARTprintf("\rwrite: %i bytes", 165);
	uint32_t size = 165;//sizeof(&g_pui8ImageFuelComp)/ sizeof(g_pui8ImageFuelComp[0]);
	res = Write24x64(0, &g_pui8ImageFuelComp[0], size);
	UARTprintf("\rwriten: %i", res);
	delayMilliseconds(10);
	uint8_t read_buf[165];
	UARTprintf("\rread: %i bytes", size);
	res = Read24x64(0, &read_buf[0], size);
	UARTprintf("\rwas read: %i", res);

	TFT_setOrientation(ORIENTATION_RIGHT2LEFT);
	GrContextForegroundSet(&g_sContext, BACKGROUND);
	GrContextBackgroundSet(&g_sContext, ClrYellow);
	GrTransparentImageDraw(&g_sContext, read_buf, 20, 200, ClrYellow);
    GrContextForegroundSet(&g_sContext, FOREGROUND);
    GrContextBackgroundSet(&g_sContext, BACKGROUND);

}

int main(void)
{
//    SetupSystemClock();
//
//    SetUpTimers();

    InitializePerepheral();

    TestInternalEEPROM();

//    UARTprintf("\rstart TestEEPROM");
//    TestEEPROM();
//    UARTprintf("\rend TestEEPROM");

    flags.update_temperature = false;
    SearchTempSensors();

//    while (1);
    while (1)
    {
        ProcessMenu();

        if(flags.update_temperature)
        {
            MeasureTemperature(0);
            flags.update_temperature = false;
            flags.update_menu = true;
        }

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
//        SchedulerRun();
    }
}
