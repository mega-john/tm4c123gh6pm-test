/*
 * internalEEPROM.h
 *
 *  Created on: 12 рту. 2014 у.
 *      Author: estarcev
 */

#ifndef INTERNALEEPROM_H_
#define INTERNALEEPROM_H_

#include "../../global.h"

void InitInternalEEPROM();

void LoadInt32(int32_t * value, uint32_t addr);
void SaveInt32(int32_t * value, uint32_t addr);
void LoadUInt32(uint32_t * value, uint32_t addr);
void SaveUInt32(uint32_t * value, uint32_t addr);

void LoadInt16(int16_t * value, uint32_t addr);
void SaveInt16(int16_t * value, uint32_t addr);
void LoadUInt16(uint16_t * value, uint32_t addr);
void SaveUInt16(uint16_t * value, uint32_t addr);

void LoadInt8(int8_t * value, uint32_t addr);
void SaveInt8(int8_t * value, uint32_t addr);
void LoadUInt8(uint8_t * value, uint32_t addr);
void SaveUInt8(uint8_t * value, uint32_t addr);

void LoadFloat(float * value, uint32_t addr);
void SaveFloat(float * value, uint32_t addr);

void LoadDouble(double * value, uint32_t addr);
void SaveDouble(double * value, uint32_t addr);

void LoadArray(uint8_t * array, uint16_t size, uint32_t addr);
void SaveArray(uint8_t * array, uint16_t size, uint32_t addr);

void TestInternalEEPROM();

#endif /* INTERNALEEPROM_H_ */
