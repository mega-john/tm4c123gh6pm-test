#include "SPI.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "../../global.h"

static unsigned long SSIBASE = 0;

static const unsigned long g_ulSSIBase[4] =
{
    SSI0_BASE,
    SSI1_BASE,
    SSI2_BASE,
    SSI3_BASE
};

static const unsigned long g_ulSSIPeriph[4] =
{
    SYSCTL_PERIPH_SSI0,
    SYSCTL_PERIPH_SSI1,
    SYSCTL_PERIPH_SSI2,
    SYSCTL_PERIPH_SSI3
};

static const unsigned long g_ulSSIConfig[4][4] =
{
    {GPIO_PA2_SSI0CLK, GPIO_PA3_SSI0FSS, GPIO_PA4_SSI0RX, GPIO_PA5_SSI0TX},
    {GPIO_PF2_SSI1CLK, GPIO_PF3_SSI1FSS, GPIO_PF0_SSI1RX, GPIO_PF1_SSI1TX},
    {GPIO_PB4_SSI2CLK, GPIO_PB5_SSI2FSS, GPIO_PB6_SSI2RX, GPIO_PB7_SSI2TX},
    {GPIO_PD0_SSI3CLK, GPIO_PD1_SSI3FSS, GPIO_PD2_SSI3RX, GPIO_PD3_SSI3TX},
};

static const unsigned long g_ulSSIPort[4] =
{
    GPIO_PORTA_BASE,
    GPIO_PORTF_BASE,
    GPIO_PORTB_BASE,
    GPIO_PORTD_BASE
};

static const unsigned long g_ulGPIOPeriph[4] =
{
    SYSCTL_PERIPH_GPIOA,
    SYSCTL_PERIPH_GPIOF,
    SYSCTL_PERIPH_GPIOB,
    SYSCTL_PERIPH_GPIOD,
};

static const unsigned long g_ulSSIPins[4] =
{ //clk, ss, rx, tx
    GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5,
    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
};

void SPI_begin(uint8_t module)
{
    uint32_t initialData = 0;
    SSIBASE = g_ulSSIBase[module];

    SysCtlPeripheralEnable(g_ulSSIPeriph[module]);
    SysCtlPeripheralEnable(g_ulGPIOPeriph[module]);
    SSIDisable(SSIBASE);
    GPIOPinConfigure(g_ulSSIConfig[module][0]);
    GPIOPinConfigure(g_ulSSIConfig[module][1]);
    GPIOPinConfigure(g_ulSSIConfig[module][2]);
    GPIOPinConfigure(g_ulSSIConfig[module][3]);
    GPIOPinTypeSSI(g_ulSSIPort[module], g_ulSSIPins[module]);
    /*
     Polarity Phase        Mode
     0 	   0   SSI_FRF_MOTO_MODE_0
     0     1   SSI_FRF_MOTO_MODE_1
     1     0   SSI_FRF_MOTO_MODE_2
     1     1   SSI_FRF_MOTO_MODE_3
     */

    SSIClockSourceSet(SSIBASE, SSI_CLOCK_SYSTEM);
    SSIConfigSetExpClk(SSIBASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, SPI_SPEED, 8);
    SSIEnable(SSIBASE);

    //clear out any initial data that might be present in the RX FIFO
    while(SSIDataGetNonBlocking(SSIBASE, &initialData)){};
}

void SPI_end()
{
    SSIDisable(SSIBASE);
}

void SPI_setDataMode(uint8_t mode)
{
    HWREG(SSIBASE + SSI_O_CR0) &= ~(SSI_CR0_SPO | SSI_CR0_SPH);
    HWREG(SSIBASE + SSI_O_CR0) |= mode;
}

void SPI_setClockDivider(uint8_t divider)
{
    HWREG(SSIBASE + SSI_O_CPSR) = divider;
}

void SPI_transfer8(uint8_t data)
{
    SSIDataPut(SSIBASE, data);
    while(SSIBusy(SSIBASE)){};
}

void SPI_transfer16(uint16_t data)
{
    SPI_transfer8(data >> 8);
    SPI_transfer8(data);
}

void SPI_transfer24(uint32_t data)
{
    SPI_transfer8(data >> 16);
    SPI_transfer8(data >> 8);
    SPI_transfer8(data);
}

void SPI_transfer32(uint32_t data)
{
    SPI_transfer8(data >> 24);
    SPI_transfer8(data >> 16);
    SPI_transfer8(data >> 8);
    SPI_transfer8(data);
}

uint8_t SPI_read8()
{
    uint32_t rxData;
    while(SSIBusy(SSIBASE)){};
    SSIDataGet(SSIBASE, &rxData);
    return (uint8_t)rxData;
}
