
#include <optical_sensor.h>
#include "board.h"
#include "log.h"

OpSensorType OpSensor[OPSEN_MAX_NUM];

void OpSensor_Init(void)
{
    memset(OpSensor, 0, sizeof(OpSensor));
    OpSensor[OPTICAL_1].eu8Name = OPTICAL_1_DEF;
    OpSensor[OPTICAL_2].eu8Name = OPTICAL_2_DEF;
}

OpticalStateType OpSensor_ReadState(OpSensorType* OpSensor)
{
    if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(InputInf[OpSensor->eu8Name].GPIOx, InputInf[OpSensor->eu8Name].Pinx))
	{
		OpSensor->eu8State = OPTICAL_ON;
	}
	else
	{
		OpSensor->eu8State = OPTICAL_OFF;
	}
		
    return OpSensor->eu8State;
}