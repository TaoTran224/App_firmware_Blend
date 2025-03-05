
#include <cylinder.h>
#include "board.h"
#include "cylinder_sensor.h"
#include "log.h"
CylinderType Cylinder[CY_MAX_NUM];

void Cylinder_Write(CylinderType* cyl, const CylinderModeType mode)
{
    if (CY_ON == mode)
    {
        HAL_GPIO_WritePin(Output[cyl->eu8Name].GPIOx, Output[cyl->eu8Name].Pinx, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(Output[cyl->eu8Name].GPIOx, Output[cyl->eu8Name].Pinx, GPIO_PIN_SET);
    }
//	cyl->eu8Mode = mode;
}

void Cylinder_Init(void)
{
    memset(Cylinder, 0, sizeof(Cylinder));
    Cylinder[CY_PULL_BILLET].eu8Name = CY_PULL_BILLET_DEF;
    Cylinder[CY_HOLD_BILLET].eu8Name = CY_HOLD_BILLET_DEF;
    Cylinder[CY_SAWN_BILLET].eu8Name = CY_SAWN_BILLET_DEF;
    Cylinder[CY_CUT_END].eu8Name = CY_CUT_END_DEF;
    //for (uint8_t j = 0; j < 10; j++)
    {
        for (uint8_t i = 0; i < CY_MAX_NUM; i++)
        {
            Cylinder_Write(&Cylinder[i], CY_OFF);
//            HAL_Delay(1000);
//            Cylinder_Write(&Cylinder[i], CY_ON);
//            HAL_Delay(1000);
        }
    }
}

//CY_PULL_BILLET_DEF = (uint8_t)0,
//CY_HOLD_BILLET_DEF = (uint8_t)1,
//CY_SAWN_BILLET_DEF = (uint8_t)2,
//CY_CUT_END_DEF = (uint8_t)3,

void Cylinder_Sawn(void)
{
//#ifdef SEND_LOG
//    SendLogStr("Cylinder_Sawn\n");
//    logLen = sprintf(log1, "CySensor_ReadState(&CySensor[CYSEN_SAWN_OFF]) = %d", HAL_GPIO_ReadPin(InputInf[17].GPIOx, InputInf[17].Pinx));
//    SendLogStr(log1);
//#endif

//    Cylinder_Write(&Cylinder[CY_SAWN_BILLET], CY_ON);
//    HAL_Delay(20);
//    while (CYSEN_OFF == CySensor_ReadState(&CySensor[CYSEN_SAWN_ON])) {;}
#ifdef SEND_LOG
    SendLogStr("Cylinder_Sawn\n");
#endif
    Cylinder_Write(&Cylinder[CY_SAWN_BILLET], CY_ON);
    HAL_Delay(50);
#ifdef SEND_LOG
    SendLogStr("Cylinder_Sawn 0\n");
#endif
	while (CYSEN_OFF == CySensor_ReadState(&CySensor[CYSEN_SAWN_ON])) {;}
        HAL_Delay(1000);
#ifdef SEND_LOG
    SendLogStr("Cylinder_Sawn 1\n");
#endif
    //#ifdef SEND_LOG
//    logLen = sprintf(log1, "CySensor_ReadState(&CySensor[CYSEN_SAWN_OFF]) = %d", HAL_GPIO_ReadPin(InputInf[17].GPIOx, InputInf[17].Pinx));
//    SendLogStr(log1);
//#endif
    Cylinder_Write(&Cylinder[CY_SAWN_BILLET], CY_OFF);
    HAL_Delay(50);
    //while (GPIO_PIN_SET == HAL_GPIO_ReadPin(InputInf[17].GPIOx, InputInf[17].Pinx)) {;}
#ifdef SEND_LOG
    SendLogStr("Cylinder_Sawn 2\n");
#endif
	while (CYSEN_OFF == CySensor_ReadState(&CySensor[CYSEN_SAWN_OFF])) {;}
  
    HAL_Delay(100);
#ifdef SEND_LOG
    SendLogStr("Cylinder_Sawn finish\n");
#endif
}

void Cylinder_CutEnd(void)
{
#ifdef SEND_LOG
    SendLogStr("Cylinder_CutEnd\n");
#endif
    Cylinder_Write(&Cylinder[CY_CUT_END], CY_ON);
    HAL_Delay(20);
    while (CYSEN_ON == CySensor_ReadState(&CySensor[CYSEN_CUT_END])) {;}
    HAL_Delay(200);
    Cylinder_Write(&Cylinder[CY_CUT_END], CY_OFF);
    HAL_Delay(200);
    while (CYSEN_OFF == CySensor_ReadState(&CySensor[CYSEN_CUT_END])) {;}
    HAL_Delay(100);
#ifdef SEND_LOG
    SendLogStr("Cylinder_CutEnd finish\n");
#endif
}
