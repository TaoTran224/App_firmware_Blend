
#ifndef SERIAL_FUNC_H_
#define SERIAL_FUNC_H_
#include "serial.h"
#include "process_data.h"
extern UARTRecType UARTRec;
extern UARTSendType UARTSend;

void SendFrameToPC(const UARTSendType* uart);
void SendResponseToPC(CmdType cmd, UARTSendType* uart);
void Process_PC_Data(void);

#endif