
#ifndef VALVE_H_
#define VALVE_H_

#include "main.h"

typedef enum
{
V_SAWN = (uint8_t)0,
V_HOLD_BILLET,
V_CUT
} ValveNameType;

typedef enum
{
V_OFF = (uint8_t)0,
V_ON
} ValveStateType;


void Valve_Control(ValveNameType name, ValveStateType state);

//void Valve_Sawn(bool state);
//void Valve_Hold_Billet(bool state);
//void Valve_Cut(void);
void Valve_Init(void);

#endif