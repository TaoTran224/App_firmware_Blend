
#include "valve.h"
#include "board.h"
#include "watchdog.h"


void Valve_Control(ValveNameType name, ValveStateType state)
{
//    switch (name)
//    {
//    case V_SAWN:
//        if (V_ON == state)
//        {
//            HAL_GPIO_WritePin(O_13_GPIO_Port, O_13_Pin, GPIO_PIN_SET);
//        }
//        else
//        {
//            HAL_GPIO_WritePin(O_13_GPIO_Port, O_13_Pin, GPIO_PIN_RESET);
//            //HAL_Delay(500);
//        }
//        break;
//
//    case V_HOLD_BILLET:
//        if (V_ON == state)
//        {
//            HAL_GPIO_WritePin(O_11_GPIO_Port, O_11_Pin, GPIO_PIN_SET);
//            HAL_GPIO_WritePin(O_15_GPIO_Port, O_15_Pin, GPIO_PIN_SET);
//        }
//        else
//        {
//            HAL_GPIO_WritePin(O_11_GPIO_Port, O_11_Pin, GPIO_PIN_RESET);
//            HAL_GPIO_WritePin(O_15_GPIO_Port, O_15_Pin, GPIO_PIN_RESET);
//        }
//        HAL_Delay(300);
//        break;
//
//    case V_CUT:
//        HAL_GPIO_WritePin(O_12_GPIO_Port, O_12_Pin, GPIO_PIN_SET);
//        HAL_GPIO_WritePin(O_16_GPIO_Port, O_16_Pin, GPIO_PIN_SET);
//        HAL_Delay(400);
//        HAL_GPIO_WritePin(O_12_GPIO_Port, O_12_Pin, GPIO_PIN_RESET);
//        HAL_GPIO_WritePin(O_16_GPIO_Port, O_16_Pin, GPIO_PIN_RESET);
//        HAL_Delay(400);
//        break;
//    }
}

//void Valve_Sawn(bool state)
//{
//	if (true == state)
//	{
//		HAL_GPIO_WritePin(O_13_GPIO_Port, O_13_Pin, GPIO_PIN_SET);
//	}
//	else
//	{
//		HAL_GPIO_WritePin(O_13_GPIO_Port, O_13_Pin, GPIO_PIN_RESET);
//        HAL_Delay(500);
//	}
//    ClearWDT();
//}
//
//void Valve_Hold_Billet(bool state)
//{
//	if (true == state)
//	{
//		HAL_GPIO_WritePin(O_11_GPIO_Port, O_11_Pin, GPIO_PIN_SET);
//	}
//	else
//	{
//		HAL_GPIO_WritePin(O_11_GPIO_Port, O_11_Pin, GPIO_PIN_RESET);
//	}
//    HAL_Delay(300);
//    ClearWDT();
//}
//
//void Valve_Cut(void)
//{
//    //uint8_t i = 0;
//
//    HAL_GPIO_WritePin(O_12_GPIO_Port, O_12_Pin, GPIO_PIN_SET);
//    HAL_Delay(500);
//    ClearWDT();
//    HAL_GPIO_WritePin(O_12_GPIO_Port, O_12_Pin, GPIO_PIN_RESET);
//    HAL_Delay(500);
//    ClearWDT();
//}

void Valve_Init(void)
{
//    HAL_GPIO_WritePin(O_13_GPIO_Port, O_13_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(O_12_GPIO_Port, O_12_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(O_11_GPIO_Port, O_11_Pin, GPIO_PIN_RESET);
}