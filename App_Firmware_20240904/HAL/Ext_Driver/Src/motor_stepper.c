
#include <motor_stepper.h>
#include "watchdog.h"
#include "board.h"
#include "serial.h"
#include "tim.h"
#include "encoder.h"
#include "log.h"

// Pin STEP 2
// 1: CLOCKWISE
// 0: ANTI_CLOCKWISE

/*Driver TB6600
ENB = On <=> disable run
ENB = Off <=> enable run

DIR = On <=> go back
DIR = Off <=> go up

*/

MotStepperType MotStepper;
void STPM_Init(void)
{
    MotStepper.eu8Enable = STPM_DISABLE;
}

void STPM_Enable(const Enable_t enable)
{
	if (STPM_ENABLE == enable)
	{
		HAL_GPIO_WritePin(Output[22].GPIOx, Output[22].Pinx, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(Output[22].GPIOx, Output[22].Pinx, GPIO_PIN_SET);
	}
}

void STPM_Dir(const Dir_t dir)
{
	if (STPM_GO_UP == dir)
	{
		HAL_GPIO_WritePin(Output[21].GPIOx, Output[21].Pinx, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(Output[21].GPIOx, Output[21].Pinx, GPIO_PIN_SET);
	}

//	if (STPM_GO_UP == dir)
//	{
//		HAL_GPIO_WritePin(Output[21].GPIOx, Output[21].Pinx, GPIO_PIN_SET);
//	}
//	else
//	{
//		HAL_GPIO_WritePin(Output[21].GPIOx, Output[21].Pinx, GPIO_PIN_RESET);
//	}
	HAL_Delay(10);



}

void STPM_PulseStart(void)
{
    MX_TIM9_Init(1000);
    HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
    __HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, 500);
}

void STPM_PulseStartPullBillet(void)
{
    MX_TIM9_Init(1000);
    HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
    __HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, 500);
}

void STPM_PulseStop(void)
{
    HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1); // stop pulse
    HAL_TIM_Base_DeInit(&htim9);
}


void STPM_Run(const Dir_t dir, const uint32_t pulse)
{
	STPM_Dir(dir);
    ENC_CountStart();
    STPM_PulseStart();
    STPM_Enable(STPM_ENABLE);
    //ENC_ReadNumPulse();
    //HAL_Delay(500); // tranh xung nguoc khi bat dau khoi dong
    if (STPM_GO_UP == dir)
    {
#ifdef SEND_LOG
        logLen = (uint8_t)sprintf(log1, "GO UP ENC_ReadNumPulse = %d\n", ENC_ReadNumPulse());
        SendLogStr(log1);
#endif        
        //while (0 == ENC_ReadNumPulse()) {;}// cho ve 0
        while ( ((4 >= ENC_ReadNumPulse()) || (0xFFFC <= ENC_ReadNumPulse())) )  {;}// kiem tra co dung quay thuan hay khong
        while (pulse >= ENC_ReadNumPulse()) {;}
        STPM_Enable(STPM_DISABLE); // off driver
#ifdef SEND_LOG
        logLen = (uint8_t)sprintf(log1, "ENC_ReadNumPulse = %d\n", ENC_ReadNumPulse());
        SendLogStr(log1);
#endif
//        while (true)
//        {
//            if (pulse <= ENC_ReadNumPulse())
//            {
//                break;
//            }
//        }
    }
    else
    {
#ifdef SEND_LOG
        logLen = sprintf(log1, "GO BACK ENC_ReadNumPulse = %d\n", ENC_ReadNumPulse());
        SendLogStr(log1);
#endif
 //   HAL_Delay(1500);
        
//        while (true)
//        {
//            if ((0xFFFF - ENC_ReadNumPulse()) >= (uint16_t)pulse)
//            {
//                break;
//            }
//        
//        }
        //while(0xFFFF == ENC_ReadNumPulse()) {;}// cho ve 0
        while ( ((0xFFFC >= ENC_ReadNumPulse()) || (4 >= ENC_ReadNumPulse())) ){;}// kiem tra co dung quay nguoc hay khong
        while ((0xFFFF - (uint16_t)pulse) <= ENC_ReadNumPulse()) {;}
        STPM_Enable(STPM_DISABLE); // off driver
#ifdef SEND_LOG
        logLen = sprintf(log1, "ENC_ReadNumPulse = %d\n", ENC_ReadNumPulse());
        SendLogStr(log1);
#endif
    }
    //STPM_Enable(STPM_DISABLE); // off driver
    STPM_PulseStop();
    ENC_CountStop();
}



