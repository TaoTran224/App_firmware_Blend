#include "led.h"
#include "board.h"


void LED_Init(void)
{
    ;
}

void LED_Write(LEDPinType pin, LEDStateType state)
{
	
    if (LED1 == pin)
    {
        if (LED_ON == state)
        {
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        }
    }
    else if (LED2 == pin)
    {
        if (LED_ON == state)
        {
            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
        }
    }   
    else if (LED3 == pin)
    {
        if (LED_ON == state)
        {
            HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
        }
    } 
}
