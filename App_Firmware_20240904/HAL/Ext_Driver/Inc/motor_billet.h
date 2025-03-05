
#ifndef MOTOR_BILLET_H_
#define MOTOR_BILLET_H_
#include "main.h"
#include "board.h"

extern MotModeType MotBillet;


void MotBillet_Init(void);
void MotBillet_Write(MotModeType mode);
#endif