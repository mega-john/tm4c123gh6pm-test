/*
 * 24x64.h
 *
 *  Created on: 20 мая 2014 г.
 *      Author: john
 */

#ifndef _24X64_H_
#define _24X64_H_

#include "../../global.h"

#define DEV_ADDR_24CXX	0x50//0xA0
#define DEV_ADDR_DS1703	0xD0
#define	I2C_MASTER_BASE I2C1_BASE
#define	SLAVE_ADDRESS  0xA0
#define	WRITE_ADDRESS  0x00
#define	DATA   0x88

void InitI2C(void);
uint32_t Read24x64(uint16_t addr, uint8_t *buff, uint32_t len);
uint32_t Write24x64(uint16_t addr, uint8_t *buff, uint32_t len);

//uint32_t I2C_Write(uint8_t dev_addr, uint16_t addr, uint8_t *buff, uint32_t len);
//uint32_t I2C_Read(uint8_t dev_addr, uint16_t addr, uint8_t *buff, uint32_t len);

#endif /* _24X64_H_ */
