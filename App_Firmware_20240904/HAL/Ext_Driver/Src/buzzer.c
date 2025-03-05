
#include <buzzer.h>
#include "board.h"

BuzzerType Buzzer[BUZZER_MAX_NUM];


void Buzzer_Write(BuzzerType* buz, const BuzzerModeType mode)
{
    if (BUZZER_ON == mode)
    {
//        if (BUZZER_EXT == buz->eu8Name)
//        {
//            HAL_GPIO_WritePin(Output[buz->eu8Name].GPIOx, Output[buz->eu8Name].Pinx, GPIO_PIN_RESET);
//        }
//        else
        {
            HAL_GPIO_WritePin(Output[buz->eu8Name].GPIOx, Output[buz->eu8Name].Pinx, GPIO_PIN_SET);
        }
    }
    else
    {
//        if (BUZZER_EXT == buz->eu8Name)
//        {
//            HAL_GPIO_WritePin(Output[buz->eu8Name].GPIOx, Output[buz->eu8Name].Pinx, GPIO_PIN_SET);
//        }
//        else
        {
            HAL_GPIO_WritePin(Output[buz->eu8Name].GPIOx, Output[buz->eu8Name].Pinx, GPIO_PIN_RESET);
        }
    }
//	buz->eu8Mode = mode;
}

void Buzzer_Init(void)
{
    memset(Buzzer, 0, sizeof(Buzzer));
    Buzzer[BUZZER_INT].eu8Name = BUZZER_INT_DEF;
    Buzzer[BUZZER_INT].eu8Mode = BUZZER_OFF;
//    Buzzer[BUZZER_EXT].eu8Name = BUZZER_EXT_DEF;
	Buzzer_Write(&Buzzer[BUZZER_INT], BUZZER_OFF);
//	Buzzer_Write(&Buzzer[BUZZER_EXT], BUZZER_OFF);
}

void Buzzer_Process(void)
{
    Buzzer_Write(&Buzzer[BUZZER_INT], Buzzer[BUZZER_INT].eu8Mode);
//    Buzzer_Write(&Buzzer[BUZZER_EXT], Buzzer[BUZZER_EXT].eu8Mode);
}