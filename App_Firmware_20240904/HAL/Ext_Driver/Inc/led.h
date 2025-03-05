#ifndef __LED_H
#define __LED_H

#include "led.h"
#include "stdio.h"
#include "stdint.h"
typedef enum
{
LED1 = (uint8_t)0,
LED2 = (uint8_t)1,
LED3 = (uint8_t)2
} LEDPinType;

typedef enum
{
LED_ON = (uint8_t)0,
LED_OFF = (uint8_t)1
} LEDStateType;

void LED_Init(void);

void LED_Write(LEDPinType pin, LEDStateType state);

#endif
