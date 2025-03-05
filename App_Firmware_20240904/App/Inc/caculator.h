
#ifndef CACULATOR_H
#define CACULATOR_H
#include "main.h"

uint8_t CalXOR(const uint8_t* data, const uint16_t offset, const uint16_t len);

bool CheckXOR(const uint8_t* data, const uint16_t offset, const uint16_t len, const uint8_t byteCheck);

uint8_t CalSUM(const uint8_t* data, const uint16_t offset, const uint16_t len);

bool CheckSUM(const uint8_t* data, const uint16_t offset, const uint16_t len, const uint8_t byteCheck);

bool SwapByte(uint8_t* data, const uint16_t length);

uint16_t CRC16(const uint8_t* buff, const uint16_t offset, const uint16_t length);

bool CheckCRC16(const uint8_t* data, const uint16_t length);

#endif 
