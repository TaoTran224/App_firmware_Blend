/*
 * process_data.h
 *
 *  Created on: Jan 16, 2022
 *      Author: TRAN NHU TAO
 */

#ifndef PROCESS_DATA_H_
#define PROCESS_DATA_H_

#include "main.h"
#include "usb_process.h"
//#include "app.h"

#define CR (uint8_t) 0x0D
#define LF (uint8_t) 0x0A

#define START_LINE (uint8_t) 0x4C //L
#define END_LINE (uint8_t) 0x0A //\n
#define END_FILE (uint8_t) 0x23 //#

#define MAX_NUM_MACHINE_PARAM_U16 (uint16_t) 20
#define U16_UART_REC_LEN (uint16_t) 1060
#define U16_UART_SEND_LEN (uint16_t) 20
//#define ROLLER_CONST (float) () // 94.5/94.2mm/ 600 xung

#define RUN_INF_MAX_STEP_U16 (uint16_t)1000

typedef enum
{
F_SEQ = 0,
F_INVALID,
F_SAWN,
F_CUT,
F_END
} CheckFrameType;

typedef enum
{
	SAWN = (uint8_t)0,
	CUT = (uint8_t)1
} ModeType;

typedef enum
{
     PC_TO_BOARD = (uint8_t)0x55,
     BOARD_TO_PC = (uint8_t)0xAA,
} DirType;

typedef enum
{
    CMD_UNKNOWN = (uint8_t)0,
    CMD_STAND_BY = (uint8_t)0x10,
    CMD_SEND_FILE = (uint8_t)0x11,
    CMD_START_RUN = (uint8_t)0x12,
    CMD_RUNNING = (uint8_t)0x13,
    CMD_PAUSE_RUN = (uint8_t)0x14,
    CMD_STOP_RUN = (uint8_t)0x15,
	CMD_REQUEST_PROG_FW = (uint8_t)0x16,
	CMD_PROG_FW = (uint8_t)0x17,
	CMD_FINISH_PROG_FW = (uint8_t)0x18,
	CMD_FINISH_SEND_FILE = (uint8_t)0xA6,
    CMD_GET_BILLET = (uint8_t)0xA7,
    CMD_WAIT_GET_BILLET = (uint8_t)0xA8,
} CmdType;


typedef struct
{
    DirType eDir;
    CmdType eCmd;
    uint8_t au8Rec[U16_UART_REC_LEN];
    uint16_t u16RecLen;
    uint8_t au8Payload[U16_UART_REC_LEN];
    uint16_t u16PayloadLen;
} __attribute__((packed)) UARTRecType;

typedef struct
{
    DirType eDir;
    CmdType eCmd;
    uint8_t au8Rec[U16_UART_SEND_LEN];
    uint16_t u16RecLen;
    uint8_t au8Payload[U16_UART_SEND_LEN];
    uint16_t u16PayloadLen;
} __attribute__((packed)) UARTSendType;

typedef enum
{
	CUT_OK = (uint8_t)0x06,
	CUT_FAIL = (uint8_t)0x15,
    CUT_FINSIH = (uint8_t)0x23
} FlagCutType;


typedef struct
{
	ModeType eMode; //che do xe ranh hoac cat cuoi
	uint16_t u16Val; //so xung encoder can quay
    uint16_t u16SeqLine; //so thu tu duong hien tai
    uint16_t u16SeqStepOfLine; //so thu tu doan trong line
}__attribute__((packed)) StepType;

typedef struct
{
	StepType saStep[RUN_INF_MAX_STEP_U16]; //thong tin buoc can chay
	uint16_t u16NumLine; //so duong lien cua file
    uint16_t u16NumStep; //so buoc can chay cua file
}__attribute__((packed)) CharactorType;

typedef struct
{
	 uint16_t NumBillet;
	 double LineLen[100]; // do dai cac duong
	 uint16_t LineInBillet[100]; // cac duong thuoc phoi stt nao
 	 uint16_t SeqBillet; // stt bat dau tu 0
	 uint16_t NumLine; // so duong
}  BilletInfType;

extern BilletInfType BilletInf;
extern CmdType Cmd_rec;

void GoToBootLoader(void);
bool GetDataFromFile(CharactorType* ch, FileType* file);
CmdType FwProcess(uint16_t* real_page, uint16_t* num_page, uint16_t* seq_page, UARTRecType* uart);
CmdType ProcessUART(UARTRecType* uart);
#endif /* INC_PROCESS_DATA_H_ */

