
#include <interrupt_process.h>
#include "app.h"
#include "usb_process.h"
#include "button_func.h"
#include "button.h"
#include "log.h"
#include "board.h"
#include "stm32f4xx_it.h"
#include "watchdog.h"
#include "encoder.h"
#include "serial.h"
#include "tim.h"
#include "usart.h"
#include "run_machine.h"

volatile uint16_t num_encoder = 0;


uint32_t i1 = 0;
uint32_t i2 = 0;


volatile uint16_t USB_wait_ms;

static uint16_t UART_timeout;
static bool UART_enableRec = false;
static bool UART_waitProcess = false;
bool UART_enableProcess = true;

static uint16_t WDT_timeout = 0;

volatile uint16_t iWait = 0;
volatile uint16_t iTimeout = 0;

volatile uint16_t iTimeout1 = 1000;
volatile uint16_t iTimeout2 = 2000;
volatile uint16_t iTimeout3 = 3000;
volatile uint16_t iTimeout0 = 0;




uint16_t alarmHandShake = 0;

uint8_t recUART3 = 0;
uint8_t recUART1 = 0;



uint8_t read[INPUT_MAX];
uint8_t k = 0;
//int: input 0-7, 9-10, 12-17




Int_FlagInType State = {.au32Value = 0};
Int_FlagInType MaskState = {.au32Value = 0};
Int_FlagInType PastState = {.au32Value = 0};

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    for (uint8_t i = 0; i <= 7; i++)
    {
        if ((GPIO_Pin == InputInf[i].Pinx) && (GPIO_PIN_RESET == HAL_GPIO_ReadPin(InputInf[i].GPIOx, InputInf[i].Pinx)))
        {
            Button[i].bIsDetectNoise = true;
            //return;
        }
    }
    for (uint8_t i = 9; i <= 10; i++)
    {
        if ((GPIO_Pin == InputInf[i].Pinx) && (GPIO_PIN_RESET == HAL_GPIO_ReadPin(InputInf[i].GPIOx, InputInf[i].Pinx)))
        {
            Button[i].bIsDetectNoise = true;
            //return;
        }
    }
    for (uint8_t i = 14; i <= 16; i++)
    {
        if ((GPIO_Pin == InputInf[i].Pinx) && (GPIO_PIN_RESET == HAL_GPIO_ReadPin(InputInf[i].GPIOx, InputInf[i].Pinx)))
        {
            Button[i].bIsDetectNoise = true;
            //return;
        }
    }
    if ((GPIO_Pin == InputInf[17].Pinx) && (GPIO_PIN_RESET == HAL_GPIO_ReadPin(InputInf[17].GPIOx, InputInf[17].Pinx)))
    {
        k++;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if (htim->Instance == htim2.Instance)
	{
		if (false == State.bits.S_START_UP)
		{
            iTimeout++;
            if (true == State.bits.S_USB_PROCESS)
            {
                USB_wait_ms++;
            }
            else
            {
                alarmHandShake++;
            }
            if (true == UART_enableRec)
			{
				if (UART_TIMEOUT_U8 < UART_timeout++)
				{
					UART_enableRec = false;
					UART_waitProcess = true;
				}
			}

//            BTN_Process();

        }
//
		if ((++i1) >= (uint32_t)100)
		{
			i1 = 0;
			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		}

        //if (true == UART_enableProcess)
        {
            if (true == UART_waitProcess)
            {
                UART_waitProcess = false;
                UART_enableProcess = false;
                UART_timeout = 0;
                State.bits.S_PROCESS_PC_DATA = true;
            }
        }

	}
	else if (htim->Instance == htim4.Instance)
	{
		if (false == State.bits.S_REQUEST_PROG_FW)
		{
			if (50 <= (++WDT_timeout))
			{
				WDT_timeout = 0;
				WDT_Clear();
			}
		}
		if ((++i2) >= (uint32_t)50)
		{
			i2 = 0;
			HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
            //HAL_GPIO_TogglePin(Output[0].GPIOx, Output[0].Pinx);

            //HAL_GPIO_TogglePin(Output[22].GPIOx, Output[22].Pinx);
		}
        Button_Detect();
            //HAL_GPIO_TogglePin(Output[21].GPIOx, Output[21].Pinx);
	}

}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    if (huart->Instance == USART1)
	{
        alarmHandShake = 0;
		UART_enableRec = true;
		UART_timeout = 0;
		UARTRec.au8Rec[UARTRec.u16RecLen++] = recUART1;

        if ((U16_UART_REC_LEN - (uint16_t)1) <= UARTRec.u16RecLen)
        {
            memset(&UARTRec, 0, sizeof(UARTRec));
        }
		HAL_UART_Receive_IT(&huart1, &recUART1, 1);
	}
}

