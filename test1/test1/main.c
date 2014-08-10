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

#define INTERNAL_EEPROM_SAVE_ADDRESS    0x3FF00

extern float Distance;

void InitInternalEEPROM()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    EEPROMInit();
    uint32_t size = EEPROMSizeGet();
//    TotalDistance = 123.45;
//    FuelSpetInTravel = 1.543;
//    unsigned long pulData[2];
//    unsigned long pulRead[2];
//    pulData[0] = 0x12345678;
//    pulData[1] = 0x56789abc;
//    FlashErase(0x10000);
//    FlashProgram((uint32_t*)&pulData, 0x10000, sizeof(pulData));
    uint32_t test=0xABCDEF;
    uint32_t result = EEPROMProgram((uint32_t*)&test, 0, 1);
    if(result != 0)
    {
        UARTprintf("\rwriten: %i", result);
    }
    test = 0xffffffff;
    EEPROMRead((uint32_t*)&test, 0, 1);
    if(test != 0xffffffff)
    {
        UARTprintf("\rwriten: %i", test);
    }
}

void InitializePerepheral()
{
    FPULazyStackingEnable();
//    SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_SYSDIV_2_5 | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    timerInit();
    InitInternalEEPROM();

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

// ///////////////////////
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
//    GPIODirModeSet(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_DIR_MODE_OUT);
//    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
/////////////////////////

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

    SetupExternalInterrupts();


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
    InitializePerepheral();

    InitInternalEEPROM();

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
