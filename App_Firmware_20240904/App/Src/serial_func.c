
#include <serial_func.h>
#include "log.h"
#include "calculator.h"
#include "interrupt_process.h"
#include "run_machine.h"
#include "usb_process.h"
#include "boot_loader.h"

UARTRecType UARTRec;
UARTSendType UARTSend;

void SendFrameToPC(const UARTSendType* uarts)
{
    uint8_t send[100];
    uint16_t send_len = 0;
    uint16_t crc = 0;
#ifdef SEND_LOG
    SendLogStr("SendFrameToPC\n");
    logLen = (uint8_t)sprintf(log1, "uart.u16PayloadLen = %d\n", uarts->u16PayloadLen);
    SendLogStr(log1);
    SendLog((uint8_t*)uarts->au8Payload, (uint16_t)uarts->u16PayloadLen);
#endif
    send_len = (uint16_t)(uarts->u16PayloadLen + (uint16_t)10);
    send[0] = 0x0D;
    send[1] = 0x0A;
    send[2] = (uint8_t)((uarts->u16PayloadLen + 2) & 0x00FF);
    send[3] = (uint8_t)((uarts->u16PayloadLen + 2) >> 8);
    send[4] = (uint8_t)BOARD_TO_PC;
    send[5] = uarts->eCmd;

    memcpy(send + 6, uarts->au8Payload, uarts->u16PayloadLen);
    crc = CRC16(send + 4, 0, uarts->u16PayloadLen + 2);
    send[send_len - 4] = (uint8_t)(crc & 0x00FF);
    send[send_len - 3] = (uint8_t)(crc >> 8);
    send[send_len - 2] = 0x0D;
    send[send_len - 1] = 0x0A;
    SerialSendToPC(send, send_len);
}

void SendResponseToPC(CmdType cmd, UARTSendType* uarts)
{
#ifdef SEND_LOG
    SendLogStr("SendResponseToPC\n");
#endif
    if (CMD_SEND_FILE == cmd)
    {
#ifdef SEND_LOG
        SendLogStr("CMD_SEND_FILE\n");
#endif
        uarts->au8Payload[2] = 0;
        uarts->au8Payload[3] = 0;
        uarts->au8Payload[8] = 0;
        uarts->au8Payload[9] = 0;
        memcpy(uarts->au8Payload, &File.u16Len, 2);
        memcpy(uarts->au8Payload + 4, &File.u16NumFrame, 2);
        memcpy(uarts->au8Payload + 6, &File.u16PreviousSeqOfFrame, 2);
        uarts->au8Payload[10] = (uint8_t)BLOCK_OK;
        uarts->u16PayloadLen = 11;
        SendFrameToPC(uarts);
        //BTN_bEnableProcess = true;
        //M_State = S_STAND_BY;
        //M_OldState = S_PROCESS_PC_DATA;
    }
    else if (CMD_FINISH_SEND_FILE == cmd)
    {
#ifdef SEND_LOG
        SendLogStr("CMD_FINISH_SEND_FILE\n");
#endif
		memset(&Charactor, 0, sizeof(Charactor));
		if (true == GetDataFromFile(&Charactor, &File))
        {
#ifdef SEND_LOG
            SendLogStr("CMD_FINISH_SEND_FILE\n");
#endif
            uarts->au8Payload[2] = 0;
            uarts->au8Payload[3] = 0;
            uarts->au8Payload[8] = 0;
            uarts->au8Payload[9] = 0;
            memcpy(uarts->au8Payload, &File.u16Len, 2);
            memcpy(uarts->au8Payload + 4, &File.u16NumFrame, 2);
            memcpy(uarts->au8Payload + 6, &File.u16PreviousSeqOfFrame, 2);
            uarts->au8Payload[10] = (uint8_t)BLOCK_FILE_OK;
            uarts->u16PayloadLen = 11;
#ifdef SEND_LOG
            SendLog(uarts->au8Payload, (uint16_t)uarts->u16PayloadLen);
#endif
            SendFrameToPC(uarts);
            memset(&RunInf, 0, sizeof(RunInf));
            memset(&File, 0, sizeof(File));
            State.bits.S_WAIT_GET_BILLET = true;
        }
        else
        {
//            M_State = S_STAND_BY;
        }
//        M_OldState = S_STAND_BY;
    }
    else if (CMD_START_RUN == cmd)
    {
        SendFrameToPC(uarts);
        if (true == PastState.bits.S_RUN_MACHINE)
        {
            State.bits.S_RUN_MACHINE = true;;
            PastState.bits.S_RUN_MACHINE = false;
        }
        else
        {
            State.bits.S_GET_BILLET = true;;
            PastState.bits.S_RUN_MACHINE = false;
        }
    }
    else if (CMD_RUNNING == cmd)
    {
#ifdef SEND_LOG
        SendLogStr("CMD_RUNNING\n");
#endif
        uarts->eDir = BOARD_TO_PC;
        uarts->eCmd = CMD_RUNNING;

        memcpy(uarts->au8Payload, &Charactor.u16NumLine, 2);
        memcpy(uarts->au8Payload + 2, &Charactor.u16NumStep, 2);
        memcpy(uarts->au8Payload + 4, &RunInf.u16SeqLine, 2);
        memcpy(uarts->au8Payload + 6, &RunInf.u16SeqStepOfLine, 2);
        uarts->au8Payload[8] = (uint8_t)CUT_OK;
        uarts->u16PayloadLen = 9;
#ifdef SEND_LOG
        SendLog(uarts->au8Payload, (uint16_t)uarts->u16PayloadLen);
#endif
        SendFrameToPC(uarts);
    }
    else if (CMD_PAUSE_RUN == cmd)
    {
#ifdef SEND_LOG
        SendLogStr("CMD_PAUSE_RUN\n");
#endif
        uarts->eDir = BOARD_TO_PC;
//        uart->eCmd = CMD_PAUSE_RUN;
//        uart->u16PayloadLen = 2;
#ifdef SEND_LOG
        SendLog(uarts->au8Payload, (uint16_t)uarts->u16PayloadLen);
#endif
        SendFrameToPC(uarts);
        //M_State = S_PAUSE_MACHINE;
        //M_OldState = S_RUN_MACHINE;
    }
	else if (CMD_REQUEST_PROG_FW == cmd)
	{
#ifdef SEND_LOG
        SendLogStr("CMD_REQUEST_PROG_FW\n");
#endif

#ifdef FW_BOOT_LOADER // gui frame phan hoi len PC
		SendFrameToPC(uarts);
#else // khong gui frame phan hoi len PC, reset MCU de MCU ve code boot loader
		BL_WriteRequestProgFW();
	    GoToBootLoader();
#endif
	}
	else if (CMD_PROG_FW == cmd)
	{
#ifdef SEND_LOG
        SendLogStr("CMD_PROG_FW\n");
#endif
#ifdef FW_BOOT_LOADER

        SendFrameToPC(uarts);
#else
		;
#endif
	}
	else if (CMD_FINISH_PROG_FW == cmd)
	{
#ifdef SEND_LOG
        SendLogStr("CMD_FINISH_PROG_FW\n");
#endif
#ifdef FW_BOOT_LOADER
		BL_ResetRequestProgFW();
        SendFrameToPC(uarts);
		BL_GotoApp();
#else
		;
#endif
	}
}


void Process_PC_Data(void)
{
    if (true == State.bits.S_PROCESS_PC_DATA)
    {
#ifdef SEND_LOG
		SendLogStr("S_PROCESS_PC_DATA\n");
#endif
        Cmd_rec = ProcessUART(&UARTRec);
        if (CMD_UNKNOWN == Cmd_rec)
        {
            memset(&File, 0, sizeof(File));
  			//M_State = M_OldState;
			//M_OldState = S_STAND_BY;
        }
        else
        {
			memset(&UARTSend, 0, sizeof(UARTSend));
			UARTSend.eCmd = UARTRec.eCmd;
            SendResponseToPC(Cmd_rec, &UARTSend);
        }
		memset(&UARTSend, 0, sizeof(UARTSend));
        memset(&UARTRec, 0, sizeof(UARTRec));
        UART_enableProcess = true;
		State.bits.S_PROCESS_PC_DATA = false;
    }
}
