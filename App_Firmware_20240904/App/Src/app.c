
#include <app.h>

#include "board.h"

#include "log.h"
#include "button_func.h"
#include "encoder.h"
#include "cylinder.h"
#include "cylinder_sensor.h"
#include "run_machine.h"
#include "serial_func.h"
#include "motor_sawn.h"
#include "motor_billet.h"
#include "usb.h"
#include "buzzer.h"
#include "motor_stepper.h"
#include "optical_sensor.h"
#include "boot_loader.h"
#include "flash.h"

void StartUp(void)
{
//	uint8_t buf[1024];
//	uint16_t i = 0;
	BoardInit();
    Buzzer_Init();
    Cylinder_Init();
    Button_Init();
    CySensor_Init();
    MotSawn_Init();
    MotBillet_Init();
	OpSensor_Init();

    MotSawn_Write(MOT_OFF);
    MotBillet_Write(MOT_OFF);
    STPM_Enable(STPM_ENABLE);
	//BL_GotoApp();
#ifdef SEND_LOG
    SendLogStr("StartUp App\n");
#endif
//	for (i = 0; i < sizeof(buf); i++)
//	{
//		buf[i] = (uint8_t)(i%0x100);
//	}
//	FL_ErasePage(FLASH_SECTOR_4);
//	FL_WritePage(67, buf, sizeof(buf));
//	memset(buf, 0, sizeof(buf));
//	FL_ReadPage(67, buf, sizeof(buf));
//#ifdef SEND_LOG
//    SendLog(buf, sizeof(buf));
//#endif
	SerialSendToPC((uint8_t*)"StartUp App\n", 12);
}

void Loop(void)
{
    Button_Process();
    Buzzer_Process();
    Get_Billet1st();
    RunMachine_Process();
    Process_PC_Data();
//    USB_Process();
//    HAL_Delay(3000);
}

void Loop_Debug(void)
{
//    uint8_t a[INPUT_MAX];
//    uint8_t b, c;
//    for (uint8_t i = 0; i < INPUT_MAX; i++)
//    {
//        a[i] = HAL_GPIO_ReadPin(InputInf[i].GPIOx, InputInf[i].Pinx);
//    }
//    b = (uint8_t)CySensor_ReadState(&CySensor[CYSEN_SAWN_ON]); //11
//    c = (uint8_t)CySensor_ReadState(&CySensor[CYSEN_SAWN_OFF]); //17
//#ifdef SEND_LOG
//    SendLogStr("Btn\n");
//    SendLog(a, INPUT_MAX);
//    SendLog(&b, 1);
//    SendLog(&c, 1);
//#endif
//    HAL_Delay(1000);

    	if (CYSEN_OFF == CySensor_ReadState(&CySensor[CYSEN_SAWN_ON]))
		{
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		}
        else
        {
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        }
}

