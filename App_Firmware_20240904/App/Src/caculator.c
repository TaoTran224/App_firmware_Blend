
#include "caculator.h"

#define CC1101_CRC16_POLY  (uint16_t)0x8005
#define CC1101_CRC16_INIT (uint16_t)0x0000

uint8_t CalXOR(const uint8_t* data, const uint16_t offset, const uint16_t len)
{
    uint8_t crc = 0;
    uint16_t i = 0;
    for (i = 0; i < len; i++)
    {
        crc ^= data[i + offset];
    }
    return crc;
}

bool CheckXOR(const uint8_t* data, const uint16_t offset, const uint16_t len, const uint8_t byteCheck)
{
    if (CalXOR(data, offset, len) == byteCheck)
        return true;
    return false;
}

uint8_t CalSUM(const uint8_t* data, const uint16_t offset, const uint16_t len)
{
    uint8_t crc = 0;
    uint16_t i = 0;
    for (i = 0; i < len; i++)
    {
        crc += data[i + offset];
    }
    return crc;
}

bool CheckSUM(const uint8_t* data, const uint16_t offset, const uint16_t len, const uint8_t byteCheck)
{
    if (CalSUM(data, offset, len) == byteCheck)
        return true;
    return false;
}

bool SwapByte(uint8_t* data, const uint16_t length)
{
    uint8_t tamp[100];
    uint16_t i = 0;
    if ((uint16_t)100 > length)
    {
        for(i = (uint16_t)0; i < length; i++)
        {
            tamp[i] = data[(uint16_t)(length - i - (uint16_t)1)];
        }
        memcpy(data, tamp, length);
        return true;
    }
    else
    {
        return false;
    }
}



static void CalCRC(const uint8_t crcData, uint16_t* crcReg)
{
	uint8_t i = 0;
	uint8_t crc = 0;
	crc = crcData;
	 for (i = (uint8_t)0; i < (uint8_t)8; i++)
	 {
	     if (((*crcReg & 0x8000) >> 8) ^ (uint16_t)(crc & (uint8_t)0x80))
	     *crcReg = (*crcReg << 1) ^ CC1101_CRC16_POLY;
	     else
	     *crcReg = (*crcReg << 1);
	     crc <<= 1;
	 }
}

uint16_t CRC16(const uint8_t* buff, const uint16_t offset, const uint16_t length)
{
	uint16_t crcresult = CC1101_CRC16_INIT;
	uint16_t i = 0;
	for (i = 0; i < length; i++)
	{
		CalCRC(buff[offset + i], &crcresult);
	}
	return crcresult;
}
    
bool CheckCRC16(const uint8_t* data, const uint16_t length)
{
	uint16_t crcRec = 0;
	uint16_t crcCal = 0;
	crcRec = CRC16(data, 0, (uint16_t)(length - (uint16_t)2));
	memcpy(&crcCal, data + length - 2, 2);
	if (crcCal == crcRec)
	{
		return true;
	}
	return false;
}
