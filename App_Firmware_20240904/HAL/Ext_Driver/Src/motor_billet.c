
#include "motor_billet.h"
#include "board.h"


MotModeType MotBillet;

void MotBillet_Init(void)
{
	MotBillet = MOT_OFF;
	HAL_GPIO_WritePin(Output[MOTOR_PULL_BILLET].GPIOx, Output[MOTOR_PULL_BILLET].Pinx, GPIO_PIN_SET);
}

void MotBillet_Write(MotModeType mode)
{
	if (MOT_ON == mode)
	{
		HAL_GPIO_WritePin(Output[MOTOR_PULL_BILLET].GPIOx, Output[MOTOR_PULL_BILLET].Pinx, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(Output[MOTOR_PULL_BILLET].GPIOx, Output[MOTOR_PULL_BILLET].Pinx, GPIO_PIN_SET);
	}
}
