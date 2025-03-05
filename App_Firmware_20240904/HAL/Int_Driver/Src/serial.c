
#include <serial.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "main.h"
#include "log.h"
#include "usb_process.h"
#include "board.h"
#include "calculator.h"
#include "process_data.h"
#include "board.h"
#include "usart.h"

void SendUART3(const uint8_t* data, const uint16_t length)
{
    HAL_UART_Transmit(&huart3, (uint8_t*)data, length, (uint32_t)((uint32_t)length*(uint32_t)3));
}

void SendUART1(const uint8_t* data, const uint16_t length)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)data, length, length);
}


void SerialSendToPC(const uint8_t* buf, const uint16_t len)
{
#ifdef SEND_LOG
    SendLogStr("SerialSendToPC\n");
    SendLog(buf, len);
#endif
	SendUART1(buf, len);
}

