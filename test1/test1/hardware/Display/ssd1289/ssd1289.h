/*
 * ssd1289.h
 *
 *  Created on: 24 но€б. 2014 г.
 *      Author: john
 */

#ifndef SSD1289_H_
#define SSD1289_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "grlib/grlib.h"

#define DISPLAY_WIDTH           239
#define DISPLAY_HEIGHT          319

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void InitDisplay(void);
extern void PixelDraw(void *pvDisplayData, int32_t x, int32_t y, uint32_t color);
extern void LineDrawH(void *pvDisplayData, int32_t x1, int32_t x2, int32_t y, uint32_t color);
extern void LineDrawV(void *pvDisplayData, int32_t x, int32_t y1, int32_t y2, uint32_t color);
//extern uint32_t ColorTranslate(void *pvDisplayData, uint32_t ui32Value);
extern const tDisplay psDisplay;


#endif /* SSD1289_H_ */
