/*
 * process_data.c
 *
 *  Created on: Jan 16, 2022
 *      Author: TRAN NHU TAO
 */


#include "process_data.h"
#include "stdlib.h"
#include "log.h"
#include "math.h"
#include "aes.h"
#include "app.h"
#include "usb_process.h"
#include "serial.h"
#include "calculator.h"
#include "watchdog.h"
#include "run_machine.h"
#include "flash.h"
#include "interrupt_process.h"
#include "boot_loader.h"

 CmdType Cmd_rec = CMD_UNKNOWN;

 BilletInfType BilletInf;
#ifdef FW_BOOT_LOADER

#else
void GoToBootLoader(void)
{
#ifdef SEND_LOG
    SendLogStr("GoToBootLoader\n");
#endif
	State.bits.S_REQUEST_PROG_FW = true;
	while(1);
}
#endif

int16_t SeekByte(const uint8_t *src, const uint16_t len, const uint8_t seek)
{
	for (uint16_t i = 0; i < len; i++)
	{
		if (seek == src[i])
		{
			return i;
		}
	}
	return (-1);
}
static bool CheckNumOK(uint16_t *dst, const CheckFrameType frametype, const uint8_t* src, const uint8_t src_len) //
{
//	uint8_t log[40];
//	uint8_t logLen = 0;
	uint8_t i = 0;
	uint8_t num_valid = 0;
	uint8_t num_dot = 0;
	double val = 0;
    //double val2 = 0;
	char str[20];

    double d2;
    double encoderPulseNum = 2400.000;
	memset(str, 0, sizeof(str));
#ifdef SEND_LOG
    SendLogStr("CheckNumOK\n");
    SendLog(src, src_len);
#endif
    d2 = 1.000 * MachineSetting.d64d2;

    if ((uint8_t)1 <= src_len)
    {
#ifdef SEND_LOG
        SendLogStr("1111111\n");
#endif
        if (((uint8_t)0x2E != src[src_len - (uint8_t)1]) && ((uint8_t)0x2E != src[(uint8_t)0]))//byte cuoi khong la dau .
        {
#ifdef SEND_LOG
            SendLogStr("2222222\n");
#endif
		    if (F_SEQ == frametype)
			{
				memcpy(str, src, src_len);
				*dst = (uint16_t)atoi(str);
#ifdef SEND_LOG
				logLen = (uint8_t)sprintf(log1, "SEQ = %d\n", *dst);
				SendLogStr(log1);
#endif
				return true;
			}
			else
			{
				for (i = (uint8_t)0; i < src_len; i++)
				{
					if (((uint8_t)0x30 <= src[i]) && ((uint8_t)0x39 >= src[i]))
					{
						num_valid++;
					}
					else if ((uint8_t)0x2E == src[i])
					{
						num_dot++;
					}
				}
#ifdef SEND_LOG
				SendLogStr("3333333\n");
				logLen = (uint8_t)sprintf(log1, "\nnum_valid = %d\n", num_valid);
				SendLogStr(log1);
				logLen = (uint8_t)sprintf(log1, "\nnum_dot = %d\n", num_dot);
				SendLogStr(log1);
#endif
				if (((num_valid + (uint8_t)1 == src_len) && ((uint8_t)1 == num_dot)) || ((num_valid == src_len) && ((uint8_t)0 == num_dot)))
				{
					memcpy(str, src, src_len);
#ifdef SEND_LOG
					SendLogStr("4444444\n");
#endif

					//val = val*157500/24649;// (val/(ROLLER_CONST));
					if ((F_SAWN == frametype) || (F_CUT == frametype))
					{
						 val = atof(str);
						*dst = (uint16_t)round(val*encoderPulseNum/d2);
					}
					//logLen = sprintf(log, "CheckNumOK = %d\n", *output);
					//SendLog( log, (uint16_t)logLen);
					return true;
				}
			}
        }
    }
    else
    {
#ifdef SEND_LOG
        SendLogStr("CheckNumOK is Invalid\n");
#endif
        return false;
    }
	return false;
}

static CheckFrameType GetLines(uint16_t* dst, const uint8_t* src, const uint8_t src_len)
{
	CheckFrameType check_frame = F_INVALID;
	const uint8_t end_character = 0x46; //F
	const uint8_t sawn = 0x56; //V
	const uint8_t cut = 0x43; //C
    const uint8_t seq = 0x53; //S
#ifdef SEND_LOG
    SendLogStr("GetLines\n");
	SendLog(src, src_len);
#endif
    if ((uint8_t)1 == src_len)
    {
        if (end_character == src[0])
        {
            check_frame = F_END;
        }
    }
    else if ((uint8_t)1 < src_len)
    {
        if (src[src_len - (uint8_t)1] == sawn)
        {
            if (true == CheckNumOK(dst, F_SAWN, src, src_len - (uint8_t)1))
            {
                check_frame = F_SAWN;
            }
        }
        else if (src[src_len - (uint8_t)1] == cut)
        {
            if (true == CheckNumOK(dst, F_CUT, src, src_len - (uint8_t)1))
            {
                check_frame = F_CUT;
            }
        }
        else if (src[src_len - (uint8_t)1] == seq)
        {
            if (true == CheckNumOK(dst, F_SEQ, src, src_len - (uint8_t)1))
            {
                check_frame = F_SEQ;
            }
        }
    }
    else if ((uint8_t)0 == src_len)
    {
#ifdef SEND_LOG
        SendLogStr("GetLines is Invalid\n");
#endif
    }
	return check_frame;
}

static double GetValSetting(const uint8_t* data, const uint16_t length)
{

    double result = 0;
#ifdef SEND_LOG
    SendLogStr("GetValSetting\n");
    SendLog(data, length);
#endif
    result = atof((char*)data);
#ifdef SEND_LOG
    logLen = (uint8_t)sprintf(log1, "result = %f\n", result);
    SendLogStr(log1);
#endif
    return result;
}

static bool GetMachineSetting(const uint8_t* data, const uint16_t length)
{
    uint16_t i = 0;
    uint16_t pos_end[MAX_NUM_MACHINE_PARAM_U16];
    uint16_t num_end = 0;
#ifdef SEND_LOG
    SendLogStr("GetMachineSetting\n");
    SendLog(data, length);
#endif
    if (6 <= length)
    {
        for (i = 0; i < length; i++) // lay vi tri ca byte #
        {
            if (END_FILE == data[i])
            {
                pos_end[num_end++] = i;
            }

            if (MAX_NUM_MACHINE_PARAM_U16 < num_end) // so dau # nhieu hon so luong cac tham so frame sai
            {
                return false;
            }
        }

        if (2 <= num_end)
        {
#ifdef SEND_LOG
            SendLogStr("GetMachineSetting\n");
#endif
            for (i = 0; i < (num_end - 1); i++)
            {
                if (5 > (pos_end[i + 1] - pos_end[i]))
                {
                    return false;
                }
                else
                {
                    if ((0 == i) && (0 == memcmp(data + pos_end[i], (uint8_t*)"#X1=", 4)))
                    {
                        MachineSetting.d64DistanceSwanToCut = GetValSetting(data + pos_end[i] + 4, pos_end[i + 1] - pos_end[i] - 4);
                    }
                    if ((1 == i) && (0 == memcmp(data + pos_end[i], (uint8_t*)"#X2=", 4)))
                    {
                        MachineSetting.d64d2 = GetValSetting(data + pos_end[i] + 4, pos_end[i + 1] - pos_end[i] - 4);
                    }
                    if ((0 < MachineSetting.d64DistanceSwanToCut) && (0 < MachineSetting.d64d2))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

static bool GetData(CharactorType* ch, const FileType* file)
{
    bool dataOK = false;
    uint16_t i = 0;
    uint16_t num_start_line = 0;
    uint16_t num_end_line = 0;
    uint16_t pos_start_line[RUN_INF_MAX_STEP_U16];
    uint16_t pos_end_line[RUN_INF_MAX_STEP_U16];
    uint16_t linesLen = 0;
    uint16_t num_valid_line = 0;

    uint16_t lines_value = 0;
    CheckFrameType linesOK = F_INVALID;
    uint8_t machine_setting[255];
    uint16_t machine_setting_len = 0;
    uint16_t seqLine = 0;
    uint16_t seqStepOfLine = 0;

#ifdef SEND_LOG
    SendLogStr("GetData\n");
    logLen = (uint8_t)sprintf(log1, "file->len = %d\n", file->u16Len);
//    SendLogStr(log1);
//    SendLog( file->au8Data, file->u16Len);
#endif
    if ((uint16_t)4 < file->u16Len)
    {
        if ((START_LINE == file->au8Data[(uint16_t)0]) &&(END_FILE == file->au8Data[file->u16Len - (uint16_t)1]))
        {
#ifdef SEND_LOG
            SendLogStr("0000000\n");
#endif
            for (i = 0; i < file->u16Len; i++)
            //for (i = 0; i < 5000; i++)
            {
                if (START_LINE == file->au8Data[i]) // lay vi tri byte L
                {
                    pos_start_line[num_start_line++] = i;
                }
                else if (END_LINE == file->au8Data[i]) // lay vi tri byte \n
                {
                    pos_end_line[num_end_line++] = i;
                }
            }
            // lay doan du lieu thong so may
            machine_setting_len = file->u16Len - pos_end_line[num_end_line - 1] - 1;
            memcpy(machine_setting, file->au8Data + pos_end_line[num_end_line - 1] + 1, machine_setting_len);
        }
#ifdef SEND_LOG
            SendLogStr("11111111\n");
#endif
        if (num_start_line == num_end_line)
        {
#ifdef SEND_LOG
            SendLogStr("Lines is Valid\n");
#endif
            for (i = 1; i < num_end_line; i++)
            {
                if ((uint16_t)1 == (pos_start_line[i] - pos_end_line[i - (uint16_t)1]))
                {
                    num_valid_line++;
                }
            }
        }

        if (true == GetMachineSetting(machine_setting, machine_setting_len))
        {
#ifdef SEND_LOG
            SendLogStr("GetMachineSetting is OK!!!\n");
#endif
            dataOK = true;
            if ((num_valid_line + (uint16_t)1) == num_start_line)
            {
                //SendLog( (uint8_t*)"\n\n(num_valid_line + 1) == num_end_line\n\n", (uint16_t)strlen("\n\n(num_valid_line + 1) == num_end_line\n\n"));
//#ifdef SEND_LOG
//                logLen = (uint8_t)sprintf(log1, "num_end_line = %d\n", num_end_line);
//                SendLogStr(log1);
//                logLen = (uint8_t)sprintf(log1, "num_start_line = %d\n", num_start_line);
//                SendLogStr(log1);
//#endif
                for (i = 0; i < num_end_line; i++)
                {
                    linesLen = pos_end_line[i] - pos_start_line[i] - (uint16_t)1;
//#ifdef SEND_LOG
//                    logLen = (uint8_t)sprintf(log1, "linesLen = %d\n", linesLen);
//                    SendLog( (uint8_t*)log1, (uint16_t)logLen);
//                    logLen = (uint8_t)sprintf(log1, "num_end_line = %d\n", num_end_line);
//                    SendLog( (uint8_t*)log1, (uint16_t)logLen);
//#endif
                    if (((uint16_t)15 > linesLen) && ((uint16_t)0 < linesLen))
                    {
                        linesOK = GetLines(&lines_value, file->au8Data + pos_start_line[i] + 1, (uint8_t)linesLen);
                        if (F_INVALID == linesOK)
                        {
                            break;
                        }
                        else if (F_SAWN == linesOK)
                        {
                            ch->saStep[ch->u16NumStep].u16SeqLine = seqLine;
                            ch->saStep[ch->u16NumStep].eMode = SAWN;
                            ch->saStep[ch->u16NumStep].u16Val = lines_value;
                            ch->saStep[ch->u16NumStep].u16SeqStepOfLine = seqStepOfLine++;
                            ch->u16NumStep ++;
                            dataOK = true;
                        }
                        else if (F_CUT == linesOK)
                        {
                            ch->saStep[ch->u16NumStep].u16SeqLine = seqLine;
                            ch->saStep[ch->u16NumStep].eMode = CUT;
                            ch->saStep[ch->u16NumStep].u16Val = lines_value;
                            ch->saStep[ch->u16NumStep].u16SeqStepOfLine = seqStepOfLine++;
                            ch->u16NumStep ++;
                            dataOK = true;
                        }
                        else if (F_SEQ == linesOK)
                        {
                            seqStepOfLine = 0;
                            ch->saStep[ch->u16NumStep].u16SeqLine = lines_value;
                            seqLine = lines_value;
                            dataOK = true;
                        }
                    }
                }
                ch->u16NumLine = seqLine + 1;
#ifdef SEND_LOG
                SendLogStr("SEND RESULT\n");
                logLen = (uint8_t)sprintf(log1, "Ch.u16NumStep = %d\n", ch->u16NumStep);
                SendLogStr(log1);
                for (i = 0; i < ch->u16NumStep; i++)
                {
                    logLen = (uint8_t)sprintf(log1, "Ch.u16NumLine = %d\n", ch->u16NumLine);
                    SendLogStr(log1);
                    logLen = (uint8_t)sprintf(log1, "Ch.aStep.u16SeqLine = %d\n", ch->saStep[i].u16SeqLine);
                    SendLogStr(log1);
//                    logLen = (uint8_t)sprintf(log1, "Ch.aStep.u16SeqStepOfLine = %d\n", ch->saStep[i].u16SeqStepOfLine);
//                    SendLogStr(log1);
//                    logLen = (uint8_t)sprintf(log1, "Ch.aStep.eMode = %d\n", ch->saStep[i].eMode);
//                    SendLogStr(log1);
//                    logLen = (uint8_t)sprintf(log1, "Ch.aStep.u16Val = %d\n", ch->saStep[i].u16Val);
//                    SendLogStr(log1);
//                    logLen = (uint8_t)sprintf(log1, "Ch.u16numStep = %d\n\n", i);
//                    SendLogStr(log1);
                }
#endif
            }
        }
    }
    else
    {
#ifdef SEND_LOG
        SendLogStr("File is small!!\n");
#endif
        return dataOK;
    }
    return dataOK;
}

static bool CheckValidData(const uint8_t* buf, const uint16_t len)
{
    uint16_t i = 0;
    uint16_t j = 0;
#ifdef SEND_LOG
//    SendLog( (uint8_t*)"CheckValidData\n", (uint16_t)strlen("CheckValidData\n"));
    //SendLog( buf, len);
#endif
    for (i = (uint16_t)0; i < (uint16_t)len; i++)
    {
        if ((((uint8_t)'L' == buf[i]) || (0x0A == buf[i]) || (0x2E == buf[i]) || ((uint8_t)'S' == buf[i]) || ((uint8_t)'F' == buf[i]) || ((uint8_t)'V' == buf[i]) ||
            ((uint8_t)'C' == buf[i]) || ((uint8_t)'#' == buf[i]) || ((uint8_t)'X' == buf[i]) || ((uint8_t)'=' == buf[i]) || ((0x30 <= buf[i]) && (0x39 >= buf[i]))) && (0 != buf[i]))
        {
            j++;
        }
        else
        {
            return false;
        }
    }
    if (len == j)
    {
//#ifdef SEND_LOG
//        SendLog( (uint8_t*)"CheckValidData is OK\n", (uint16_t)strlen("CheckValidData is OK\n"));
//#endif
        return true;
    }
    return false;
}

static CmdType GetUART_File(FileType* file, UARTRecType* uartr)
{
    uint16_t file_len = 0;
    uint16_t num_frame = 0;
    uint16_t real_seq = 0;
    uint16_t block_size = 0;
    uint16_t block_size_1st = 0;
    uint16_t block_size_end = 0; // do dai du lieu trong frame cuoi cung

    uint8_t block_data[U16_UART_REC_LEN];
    CmdType cmd = CMD_UNKNOWN;

#ifdef SEND_LOG
    SendLogStr("GetUART_File\n");
    SendLog(uartr->au8Payload, uartr->u16PayloadLen);
#endif
    // Check du lieu dau vao
    uartr->eCmd = CMD_UNKNOWN;
    file->eIsBlockChecked = BLOCK_FAIL;
    if ((uint16_t)16 <= uartr->u16PayloadLen)
    {
        memcpy(&file_len, uartr->au8Payload, 2);
        memcpy(&num_frame, uartr->au8Payload + 4, 2);
        memcpy(&real_seq, uartr->au8Payload + 6, 2);
        memcpy(&block_size, uartr->au8Payload + 8, 2);
#ifdef SEND_LOG
        logLen = (uint8_t)sprintf(log1, "file_len = %d\n", file_len);
        SendLogStr(log1);
        logLen = (uint8_t)sprintf(log1, "num_frame = %d\n", num_frame);
        SendLogStr(log1);
        logLen = (uint8_t)sprintf(log1, "real_seq = %d\n", real_seq);
        SendLogStr(log1);
        logLen = (uint8_t)sprintf(log1, "block_size = %d\n", block_size);
        SendLogStr(log1);
#endif
        if (((uint16_t)0 == (uint16_t)(block_size%(uint16_t)16)) && ((block_size + 10) == uartr->u16PayloadLen))
        {
#ifdef SEND_LOG
            SendLogStr("Format frame OKOKOK\n");
#endif
            aes_decrypt(block_data, uartr->au8Payload + 10, block_size, 0, 0);
            if (((uint16_t)0 == file->u16NumFrame) && ((uint16_t)0 == real_seq)) // nhan frame dau tien
            {
#ifdef SEND_LOG
                SendLogStr("FIRST frame\n");
                SendLog((uint8_t*)block_data, (uint16_t)block_size);
#endif
                if ((uint16_t)1 == num_frame) // chi co 1 frame duy nhat
                {
                    memcpy(&block_size_1st, block_data, 2);
#ifdef SEND_LOG
                    SendLogStr("Format only 1 frame\n");
                    logLen = (uint8_t)sprintf(log1, "block_size_1st = %d\n", block_size_1st);
                    SendLogStr(log1);
#endif
                    if (block_size_1st == file_len) // file_length = 2 byte do dai trong du lieu ma hoa
                    {
                        if (true == CheckValidData(block_data + 2, file_len))
                        {
                            memset(file, 0, sizeof(FileType));
                            file->u16Len = block_size_1st;//file_len;
#ifdef SEND_LOG
                            SendLog( (uint8_t*)"Format only 1 frame OKOKOK\n", 28);
                            logLen = (uint8_t)sprintf(log1, "file_len = %d\n", file_len);
                            SendLogStr(log1);
                            logLen = (uint8_t)sprintf(log1, "block_size_1st = %d\n", block_size_1st);
                            SendLogStr(log1);
                            logLen = (uint8_t)sprintf(log1, "file->u16Len = %d\n", file->u16Len);
                            SendLogStr(log1);
#endif
                            memcpy(file->au8Data, block_data + 2, file->u16Len);
                            file->u16NumFrame = num_frame;
                            file->u16PreviousSeqOfFrame = real_seq;
                            uartr->eCmd = CMD_SEND_FILE;
                            file->eIsBlockChecked = BLOCK_OK;
                            cmd = CMD_FINISH_SEND_FILE;
#ifdef SEND_LOG
                            SendLog( file->au8Data, file->u16Len);
#endif
                        }
                    }
                }
                else if ((uint16_t)1 < num_frame) // co tu 2 frame tro len
                {
                    memset(file, 0, sizeof(FileType));
                    memcpy(file->au8Data, block_data, block_size);
                    file->u16Len = file_len;
                    file->u16NumFrame = num_frame;
                    file->u16PreviousSeqOfFrame = real_seq;
                    uartr->eCmd = CMD_SEND_FILE;
                    file->eIsBlockChecked = BLOCK_OK;
                    cmd = CMD_SEND_FILE;
#ifdef SEND_LOG
                    SendLogStr("MANY frames OKOKOK\n");
                    SendLogStr("FIRST frame\n");
                    SendLog(file->au8Data + file->u16RealLen, block_size);
#endif
                    file->u16RealLen = block_size;
                }
            }
            else if ((uint16_t)0 < file->u16NumFrame) // nhan frame thu 2 tro len
            {
#ifdef SEND_LOG
                SendLog( (uint8_t*)block_data, (uint16_t)block_size);
#endif
                if (real_seq == ((uint16_t)1 + file->u16PreviousSeqOfFrame)) // kiem tra xem stt frame hien tai co lon hon truoc 1 don vi hay ko
                {
                    if (((uint16_t)1 + real_seq) == num_frame) // neu la frame cuoi cung
                    {
                        memcpy(&block_size_end, block_data, 2);
                        memcpy(file->au8Data + file->u16RealLen, block_data + 2, block_size_end);
                        file->u16PreviousSeqOfFrame = real_seq;
                        file->eIsBlockChecked = BLOCK_OK;
                        uartr->eCmd = CMD_SEND_FILE;
                        cmd = CMD_FINISH_SEND_FILE;
#ifdef SEND_LOG
                        logLen = (uint8_t)sprintf(log1, "block_size_end = %d\n", block_size_end);
                        SendLogStr(log1);
                        SendLog( file->au8Data + file->u16RealLen, block_size_end);
                        logLen = (uint8_t)sprintf(log1, "FINISH frame\n real_seq = %d\n", real_seq);
                        SendLogStr(log1);
                        SendLogStr("ALL FRAME\n");
                        SendLog( file->au8Data, file->u16Len);
#endif
                    }
                    else if ((uint16_t)1 < (num_frame - real_seq))
                    {
                        memcpy(file->au8Data + file->u16RealLen, block_data, block_size);
                        file->u16PreviousSeqOfFrame = real_seq;
                        file->eIsBlockChecked = BLOCK_OK;
                        uartr->eCmd = CMD_SEND_FILE;
                        cmd = CMD_SEND_FILE;
#ifdef SEND_LOG
                        logLen = (uint8_t)sprintf(log1, "CONTINUE frame\n real_seq = %d\n", real_seq);
                        SendLog( (uint8_t*)log1, (uint16_t)logLen);
                        SendLog( file->au8Data + file->u16RealLen, block_size);
#endif
                        file->u16RealLen += block_size;
                    }
                }
            }
        }
    }
    else
    {
#ifdef SEND_LOG
        SendLogStr("Frame is Short\n");
#endif
        return cmd;
    }
#ifdef SEND_LOG
    SendLogStr("GetUART_File_Finish\n");
#endif
    return cmd;
}

CmdType ProcessUART(UARTRecType* uartr)
{
	CmdType cmd = CMD_UNKNOWN;
    uint16_t payload_len = 0;
#ifdef SEND_LOG
    SendLogStr("ProcessUART\n");
    logLen = (uint8_t)sprintf(log1, "uart->u16RecLen = %d\n", uartr->u16RecLen);
    SendLogStr(log1);
    SendLog(uartr->au8Rec, uartr->u16RecLen);
#endif

    payload_len = (uint16_t)(uartr->au8Rec[2]) + ((uint16_t)uartr->au8Rec[3] << 8);
#ifdef SEND_LOG
    logLen = (uint8_t)sprintf(log1, "payload_len = %d\n", payload_len);
    SendLogStr(log1);
#endif
    if ((uartr->u16RecLen == (8 + payload_len)) && (0x0D == uartr->au8Rec[0]) && (0x0A == uartr->au8Rec[1]) && (0x0D == uartr->au8Rec[uartr->u16RecLen - 2]) &&
		(0x0A == uartr->au8Rec[uartr->u16RecLen - 1]) && (true == CheckCRC16(uartr->au8Rec + 4, uartr->u16RecLen - 6)))
    {
#ifdef SEND_LOG
        SendLogStr("ProcessUART\n");
#endif
        if ((uint8_t)PC_TO_BOARD == uartr->au8Rec[4])
        {
            uartr->eDir = PC_TO_BOARD;
#ifdef SEND_LOG
            SendLogStr("ProcessUART\n");
#endif
            if (((uint8_t)CMD_STAND_BY == uartr->au8Rec[5]) && (2 == payload_len))
            {
                uartr->eCmd = CMD_STAND_BY;
                cmd = CMD_STAND_BY;
#ifdef SEND_LOG
                SendLogStr("CMD_STAND_BY\n");
#endif
            }
            else if (((uint8_t)CMD_START_RUN == uartr->au8Rec[5]) && (2 == payload_len))
            {
                uartr->eCmd = CMD_START_RUN;
                cmd = CMD_START_RUN;
#ifdef SEND_LOG
                SendLogStr("CMD_START_RUN\n");
#endif
            }
            else if (((uint8_t)CMD_PAUSE_RUN == uartr->au8Rec[5]) && (2 == payload_len))
            {
                uartr->eCmd = CMD_PAUSE_RUN;
                cmd = CMD_PAUSE_RUN;
#ifdef SEND_LOG
                SendLogStr("CMD_PAUSE_RUN\n");
#endif
            }
            else if (((uint8_t)CMD_STOP_RUN == uartr->au8Rec[5]) && (2 == payload_len))
            {
                uartr->eCmd = CMD_STOP_RUN;
                cmd = CMD_STOP_RUN;
#ifdef SEND_LOG
                SendLogStr("CMD_STOP_RUN\n");
#endif
            }
            else if (((uint8_t)CMD_RUNNING == uartr->au8Rec[5]) && (2 == payload_len))
            {
                uartr->eCmd = CMD_RUNNING;
                cmd = CMD_RUNNING;
#ifdef SEND_LOG
                SendLogStr("CMD_RUNNING\n");
#endif
            }
            else if (((uint8_t)CMD_SEND_FILE == uartr->au8Rec[5]) && (2 < payload_len))
            {
                uartr->eCmd = CMD_SEND_FILE;
                cmd = CMD_SEND_FILE;
#ifdef SEND_LOG
                SendLogStr("SEND_FILE\n");
#endif
                uartr->u16PayloadLen = payload_len - 2;
                memcpy(uartr->au8Payload, uartr->au8Rec + 6, uartr->u16PayloadLen);
#ifdef SEND_LOG
                logLen = (uint8_t)sprintf(log1, "uart->u16PayloadLen = %d\n", uartr->u16PayloadLen);
                SendLogStr(log1);
                SendLog(uartr->au8Payload, uartr->u16PayloadLen);
#endif
                cmd = GetUART_File(&File, uartr);
            }
            else if (((uint8_t)CMD_REQUEST_PROG_FW == uartr->au8Rec[5]) && (2 == payload_len))
            {
#ifdef SEND_LOG
                SendLogStr("CMD_REQUEST_PROG_FW\n");
#endif
                uartr->eCmd = CMD_REQUEST_PROG_FW;
                cmd = CMD_REQUEST_PROG_FW;
			}
            else if (((uint8_t)CMD_PROG_FW == uartr->au8Rec[5]) && (2 < payload_len))
            {
#ifdef SEND_LOG
                SendLogStr("CMD_PROG_FW\n");
#endif
                uartr->eCmd = CMD_PROG_FW;
                cmd = CMD_PROG_FW;
                uartr->u16PayloadLen = payload_len - 2;
                memcpy(uartr->au8Payload, uartr->au8Rec + 6, uartr->u16PayloadLen);
#ifdef SEND_LOG
                logLen = (uint8_t)sprintf(log1, "uart->u16PayloadLen = %d\n", uartr->u16PayloadLen);
                SendLogStr(log1);
                SendLog(uartr->au8Payload, uartr->u16PayloadLen);
#endif
                cmd = FwProcess(&FW_u16RealAddrPage, &FW_u16NumPage, &FW_u16SeqPage, uartr);
			}
			else
            {
#ifdef SEND_LOG
                SendLogStr("UART_UNKNOWN_FRAME\n");
#endif
            }
        }
    }
#ifdef SEND_LOG
    SendLogStr("ProcessUART_Finish\n");
#endif
    return cmd;
}

bool GetDataFromFile(CharactorType* ch, FileType* file)
{
#ifdef SEND_LOG
    SendLogStr("GetDataFromFile\n");
#endif
	if (true == GetData(ch, file))
	{
#ifdef SEND_LOG
        SendLogStr("GetDataOKOK\n");
#endif
        File.eIsBlockChecked = BLOCK_FILE_OK;
		return true;
	}
    File.eIsBlockChecked = BLOCK_FILE_FAIL;
	return false;
}

CmdType FL_ProgFW(uint8_t* buf, uint16_t len, uint16_t real_page, uint16_t num_page, uint16_t seq_page)
{
	CmdType cmd = CMD_UNKNOWN;
	uint8_t num_sector = (uint8_t)(num_page / 128) + (((num_page % 128) == 0) ? 0 : 1);
#ifdef SEND_LOG
        SendLogStr("FL_ProgFW\n");
#endif
	if (0 == seq_page)
	{
		for (uint8_t sector = 0; sector < num_sector; sector++)
		{
#ifdef SEND_LOG
        	SendLogStr("FL_EraseSector\n");
#endif
			FL_EraseSector(FL_SECTOR_FW_APP + sector);
		}
	}
	FL_WritePage(real_page, buf, len);
	cmd = CMD_PROG_FW;
	if ((seq_page + 1) == num_page)   // frame cuoi cung, nap xong fw
	{
#ifdef SEND_LOG
        	SendLogStr("Last firmware\n");
#endif
		cmd = CMD_FINISH_PROG_FW;
	}
	return cmd;
}

CmdType FwProcess(uint16_t* real_page, uint16_t* num_page, uint16_t* seq_page, UARTRecType* uartr)
{
	 CmdType cmd = CMD_UNKNOWN;
	 uint8_t header[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#ifdef SEND_LOG
        SendLogStr("FwProcess\n");
#endif
	uint8_t block_data[FLASH_BLOCK_SIZE + 16];
	//uint16_t len = 0;
	if ((FLASH_BLOCK_SIZE + 16) == uartr->u16PayloadLen)
	{
		aes_decrypt(block_data, uartr->au8Payload, uartr->u16PayloadLen, 0, 0);
		memcpy(num_page, block_data + 10, 2);
		memcpy(seq_page, block_data + 12, 2);
		memcpy(real_page, block_data + 14, 2);
#ifdef SEND_LOG
            logLen = (uint8_t)sprintf(log1, "num_page = %d\n", *num_page);
            SendLogStr(log1);
            logLen = (uint8_t)sprintf(log1, "seq_page = %d\n", *seq_page);
            SendLogStr(log1);
            logLen = (uint8_t)sprintf(log1, "real_page = %d\n", *real_page);
            SendLogStr(log1);
#endif
		if ((0 == memcmp(header, block_data, sizeof(header))) && (*seq_page < *num_page) && (336 > *num_page) && (128 <= *real_page) && (512 > *real_page))
		{
			//memcpy(buf, block_data + 16, FLASH_BLOCK_SIZE);
//#ifdef SEND_LOG
//        	SendLog(block_data, uartr->u16PayloadLen);
//#endif
			cmd = FL_ProgFW(block_data + 16, FLASH_BLOCK_SIZE, *real_page, *num_page, *seq_page);
		}
	}

    return cmd;
}