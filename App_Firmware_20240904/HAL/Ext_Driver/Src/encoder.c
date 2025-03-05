
#include <encoder.h>
#include "tim.h"


void ENC_CountStart(void)
{
    MX_TIM3_Init();
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2);
}

uint16_t ENC_ReadNumPulse(void)
{
    return __HAL_TIM_GET_COUNTER(&htim3);
}

void ENC_CountStop(void)
{
    HAL_TIM_Encoder_Stop(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2);
    HAL_TIM_Encoder_DeInit(&htim3);
}

//    HAL_GPIO_WritePin(OUT21_GPIO_Port, OUT21_Pin, GPIO_PIN_SET);

//    encoder = __HAL_TIM_GET_COUNTER(&htim3);
//#ifdef SEND_LOG
//    logLen = (uint8_t)sprintf(log1, "encoder = %d\n", encoder);
//    SendLogStr(log1);
//#endif
//  //pwm
//    MX_TIM9_Init();
//    HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
//    __HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, 500);
//
//    HAL_GPIO_WritePin(OUT22_GPIO_Port, OUT22_Pin, GPIO_PIN_RESET);
//    HAL_Delay(10);
//    for (uint8_t i = 0; i < 3; i ++)
//    {
//        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//        HAL_Delay(30000);
//        encoder = __HAL_TIM_GET_COUNTER(&htim3);
//
//#ifdef SEND_LOG
//        logLen = (uint8_t)sprintf(log1, "encoder = %d\n", encoder);
//        SendLogStr(log1);
//#endif
//    }
//    HAL_GPIO_WritePin(OUT22_GPIO_Port, OUT22_Pin, GPIO_PIN_SET);
//
//    //PWM
//    HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
//    HAL_TIM_PWM_DeInit(&htim9);
//    HAL_Delay(30);
//    
////        //encoder
//    HAL_TIM_Encoder_Stop(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2);
//    HAL_TIM_Encoder_DeInit(&htim3);
      
   
    

//    HAL_GPIO_TogglePin(OUT21_GPIO_Port, OUT21_Pin);	


