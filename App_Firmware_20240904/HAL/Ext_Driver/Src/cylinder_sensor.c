
#include <cylinder_sensor.h>
#include "board.h"
#include "log.h"
CySensorType CySensor[CYSEN_MAX_NUM];

CySensorStateType CySensor_ReadState(CySensorType* cysen)
{
//#ifdef SEND_LOG
//    logLen = sprintf(log1, "cysen->eu8Name = %d\n", (uint8_t)cysen->eu8Name);
//    SendLogStr(log1);
//#endif
    if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(InputInf[cysen->eu8Name].GPIOx, InputInf[cysen->eu8Name].Pinx))
	{
		cysen->eu8State = CYSEN_ON;
	}
	else
	{
		cysen->eu8State = CYSEN_OFF;
	}
	return cysen->eu8State;
}

void CySensor_Init(void)
{
    memset(CySensor, 0, sizeof(CySensor));
    CySensor[CYSEN_PULL_BILLET].eu8Name = CYSEN_PULL_BILLET_DEF;
	CySensor[CYSEN_SAWN_OFF].eu8Name = CYSEN_SAWN_OFF_DEF;
	CySensor[CYSEN_SAWN_ON].eu8Name = CYSEN_SAWN_ON_DEF;
	CySensor[CYSEN_HOLD_BILLET].eu8Name = CYSEN_HOLD_BILLET_DEF;
	CySensor[CYSEN_CUT_END].eu8Name = CYSEN_CUT_END_DEF;
}
