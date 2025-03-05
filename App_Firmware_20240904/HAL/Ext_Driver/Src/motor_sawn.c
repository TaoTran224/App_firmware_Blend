#include <motor_sawn.h>
#include "board.h"

MotModeType MotSawn;

void MotSawn_Init(void)
{
	MotSawn = MOT_OFF;
	HAL_GPIO_WritePin(Output[MOTOR_SAWN].GPIOx, Output[MOTOR_SAWN].Pinx, GPIO_PIN_SET);
}

void MotSawn_Write(MotModeType mode)
{
	if (MOT_ON == mode)
	{
		HAL_GPIO_WritePin(Output[MOTOR_SAWN].GPIOx, Output[MOTOR_SAWN].Pinx, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(Output[MOTOR_SAWN].GPIOx, Output[MOTOR_SAWN].Pinx, GPIO_PIN_SET);
	}
}
