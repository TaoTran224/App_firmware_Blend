
#ifndef __INTERRUPT_PROCESS_H_
#define __INTERRUPT_PROCESS_H_

#include "main.h"
#include "stdbool.h"
#include "serial.h"
#include "board.h"

#define MAX_USB_WAIT_MS (uint16_t) 6000

//#define MAX_ALARM_HANDSHAKE (uint16_t) 1500

#define WDT_MAX_TIME_U16 (uint16_t) 500

typedef struct
{
uint8_t S_START_UP : 1;
uint8_t S_STAND_BY : 1;
uint8_t S_PROCESS_PC_DATA : 1;
uint8_t S_SET_UP_MACHINE : 1;
uint8_t S_USB_PROCESS : 1;
uint8_t S_GET_BILLET : 1;
uint8_t S_WAIT_GET_BILLET : 1;
uint8_t S_STOP_MACHINE : 1;
uint8_t S_PAUSE_MACHINE : 1;
uint8_t S_RUN_MACHINE : 1;
uint8_t S_READ_BUTTON : 1;
uint8_t S_PROCESS_BUTTON : 1;
uint8_t S_EMERGENCY_MACHINE : 1;
uint8_t S_TEST_STEPER_MOTOR : 1;
uint8_t S_TEST_CUT_MOTOR : 1;
uint8_t S_TEST_HOLD_BILLET : 1;
uint8_t S_REQUEST_PROG_FW : 1;
uint8_t S_PROG_FW : 1;
} BITS;

typedef union
{
    BITS bits;
    uint32_t au32Value;

} Int_FlagInType;

extern volatile uint16_t USB_wait_ms;


//extern uint16_t UART_timeout;
//extern bool UART_enableRec;
//extern bool UART_waitProcess;
extern bool UART_enableProcess;


extern uint8_t recUART3;
extern uint8_t recUART1;

extern uint8_t read[INPUT_MAX];

extern uint16_t alarmHandShake;

extern volatile uint16_t iWait;
extern volatile uint16_t iTimeout;



extern Int_FlagInType State;
extern Int_FlagInType MaskState;
extern Int_FlagInType PastState;


#endif
