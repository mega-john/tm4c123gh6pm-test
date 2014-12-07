/*
 * ssd1289.h
 *
 *  Created on: 24 но€б. 2014 г.
 *      Author: john
 */

#ifndef SSD1289_H_
#define SSD1289_H_

//#include <stdio.h>
//#include <stdbool.h>
//#include <stdint.h>
//#include <string.h>
//#include <float.h>
//#include <math.h>
//
//#include "inc/hw_gpio.h"
//#include "inc/hw_memmap.h"
//#include "inc/hw_sysctl.h"
//#include "inc/hw_types.h"
//#include "inc/hw_hibernate.h"
//#include "inc/hw_i2c.h"
//#include "inc/hw_ssi.h"
//#include "inc/hw_ints.h"
//#include "inc/hw_nvic.h"
//#include "inc/hw_timer.h"
////#include "inc/hw_onewire.h"
//
//#include "driverlib/gpio.h"
//#include "driverlib/sysctl.h"
//#include "driverlib/timer.h"
//#include "driverlib/pin_map.h"
//#include "driverlib/ssi.h"
//#include "driverlib/fpu.h"
//#include "driverlib/i2c.h"
//#include "driverlib/interrupt.h"
//#include "driverlib/uart.h"
//#include "driverlib/systick.h"
//#include "driverlib/onewire.h"
//#include "driverlib/eeprom.h"
//#include "driverlib/flash.h"
//#include "driverlib/onewire.h"
//#include "utils/softi2c.h"
//#include "utils/spi_flash.h"
//#include "utils/ustdlib.h"
//#include "utils/uartstdio.h"
//#include "grlib/grlib.h"
//
//


//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern const tDisplay g_sSSD1289;
extern void SSD1289Init(void);
void TFT_Clear(unsigned int color);
void LineDrawV(void *pvDisplayData, int32_t x, int32_t y1, int32_t y2, uint32_t color);
void LineDrawH(void *pvDisplayData, int32_t x1, int32_t x2, int32_t y, uint32_t color);

#endif /* SSD1289_H_ */
