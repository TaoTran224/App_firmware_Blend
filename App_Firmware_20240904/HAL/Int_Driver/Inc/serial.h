#ifndef __SERIAL_H
#define __SERIAL_H
#include "main.h"

void SendUART3(const uint8_t* data, const uint16_t length);
void SendUART1(const uint8_t* data, const uint16_t length);

void SerialSendToPC(const uint8_t* buf, const uint16_t len);

#endif
