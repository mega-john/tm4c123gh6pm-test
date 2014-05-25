/*
 * 24x64.h
 *
 *  Created on: 20 мая 2014 г.
 *      Author: john
 */

#ifndef _24X64_H_
#define _24X64_H_

#include "../../global.h"

#define DEV_ADDR_24CXX	0xA0
#define DEV_ADDR_DS1703	0xD0

void Init24x64(void);
uint8_t Read24x64Byte();
void Write24x64Byte(uint32_t address, uint8_t data);

#endif /* _24X64_H_ */
