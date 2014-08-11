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

void InternalEEPROMWrite(unsigned char * pucBuffer, int iLength, int iOffset)
{
    //
    // Write the info to the EEPROM.
    //
    EEPROMProgram((uint32_t *)pucBuffer, (uint32_t)(0 + iOffset), (uint32_t)iLength);
}

void InternalEEPROMRead(unsigned char * pucBuffer, int iLength, int iOffset)
{
    //
    // Read the requested data.
    //
    EEPROMRead((uint32_t *)pucBuffer, (uint32_t)(0 + iOffset), (uint32_t)iLength);
}

void InitInternalEEPROM()
{
    //
    // Enable the EEPROM peripheral.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);

    //
    // Initialize the EEPROM
    //
    EEPROMInit();
}

void TestInternalEEPROM()
{
	UARTprintf("\n init internal EEPROM");
	InitInternalEEPROM();

//	EEPROMMassErase();
//	unsigned long pulRead[2];
//	int writeData=0xabcdefff;
	const unsigned char writeData[6] = {0x11,0x22,0x33,0x44,0x55,0x66};
	int size = sizeof(writeData) + 2;

//	UARTprintf("\n write to internal EEPROM");
//	InternalEEPROMWrite((unsigned char *)writeData, size, 0);
	unsigned char readData[6] = {0};
	UARTprintf("\n read from internal EEPROM");
	InternalEEPROMRead((unsigned char *)readData, size, 0);
	uint8_t i = 0;
	UARTprintf("\n result: ", readData[i]);
	for(;i < size; i++)
	{
		UARTprintf("0x%x ", readData[i]);
	}

	float f = 0.123456789;
	UARTprintf("\n write to internal EEPROM float %4.4f", f);
	InternalEEPROMWrite((unsigned char *)&f, sizeof(float), 0);
	UARTprintf("\n read from internal EEPROM");
	float ff = 0;
	InternalEEPROMRead((unsigned char *)&ff, sizeof(float), 0);
	UARTprintf("\n result: %4.4f", ff);
}

void InitializePerepheral()
{
    FPULazyStackingEnable();

    timerInit();

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
