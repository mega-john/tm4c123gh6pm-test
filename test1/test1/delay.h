/*
 * delay.h
 *
 *  Created on: 05 θώνÿ 2014 γ.
 *      Author: estarcev
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "global.h"

#define SYSTICKMS               (1000 / SYSTICKHZ)
#define SYSTICKHZ               100
#define SYSTICK_INT_PRIORITY    0x80

void delayMicroseconds(uint32_t microseconds);
void delayMilliseconds(uint32_t milliseconds);

#endif /* DELAY_H_ */
