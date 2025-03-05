
#include <button.h>
#include "interrupt_process.h"
#include "watchdog.h"
#include "main.h"
#include "board.h"
#include "log.h"


ButtonType Button[BTN_MAX_NUM];

void BTN_ClearPin(ButtonType* btn)
{
    btn->u16TimeNoise = 0;
    btn->bIsDetectNoise = false;
    btn->bIsPress = false;
}

void BTN_ClearAllPin(void)
{
    ;
}




void BTN_DetectPin(ButtonType* btn, uint16_t timeout)
{  
    if (true == btn->bIsDetectNoise)
    {
        if (timeout <= (btn->u16TimeNoise++))
        {
            if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(InputInf[btn->eu8Name].GPIOx, InputInf[btn->eu8Name].Pinx))
            {
                if (false == btn->bIsPress)
                {
                    btn->bIsPress = true;
                }
                btn->bIsDetectNoise = false;
            }
            else
            {
                BTN_ClearPin(btn);
            }
            btn->bIsDetectNoise = false;
        }
    }
}

bool BTN_IsPress(const ButtonType* btn)
{
    if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(InputInf[btn->eu8Name].GPIOx, InputInf[btn->eu8Name].Pinx))
    {
        return true;
    }
    else
    {
        return false;
    }
}

