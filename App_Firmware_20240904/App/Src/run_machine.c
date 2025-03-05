
#include <run_machine.h>
#include "log.h"
#include "motor_billet.h"
#include "motor_sawn.h"
#include "motor_stepper.h"
#include "cylinder.h"
#include "interrupt_process.h"
#include "CLCD_I2C.h"
#include "serial_func.h"
#include "optical_sensor.h"
RunInfType RunInf;


MachineSettingType MachineSetting;


CharactorType Charactor;

bool turn_on_step = false;
bool turn_on_cut = false;

uint16_t encoder = 0;

static void PullBillet(const uint16_t seq)
{
    uint16_t encoder_pulse = 0;
    uint16_t distanceSwanToCut = 0;
    distanceSwanToCut = (uint16_t)(MachineSetting.d64DistanceSwanToCut * 1.000);
#ifdef SEND_LOG
    SendLogStr("PullBillet\n");
    logLen = (uint8_t)sprintf(log1, "seq = %d\n", seq);
    SendLogStr(log1);
    logLen = (uint8_t)sprintf(log1, "distanceSwanToCut = %d\n", distanceSwanToCut);
    SendLogStr(log1);
#endif

    if ((true == RunInf.bPreviousIsCut) && ( 0 < seq))
    {
#ifdef SEND_LOG
        SendLogStr("previousIsCut\n");
#endif
        if (Charactor.saStep[seq].u16Val >= Charactor.saStep[seq - 1].u16Val)// khong can phai lui lai sau khi cat cuoi
        {
            MotStepper.eu8Dir = STPM_GO_UP;
            encoder_pulse = Charactor.saStep[seq].u16Val - Charactor.saStep[seq - 1].u16Val;
#ifdef SEND_LOG
            SendLogStr("Go ahead\n");
            logLen = (uint8_t)sprintf(log1, "encoder_pulse = %d\n", encoder_pulse);
            SendLogStr(log1);
#endif
        }
        else if (Charactor.saStep[seq].u16Val < Charactor.saStep[seq - 1].u16Val)// phai lui phoi lai
        {
            MotStepper.eu8Dir = STPM_GO_BACK;
            encoder_pulse = Charactor.saStep[seq - 1].u16Val - Charactor.saStep[seq].u16Val; //distanceSwanToCut;
#ifdef SEND_LOG
            SendLogStr("Go back\n");
            logLen = (uint8_t)sprintf(log1, "encoder_pulse = %d\n", encoder_pulse);
            SendLogStr(log1);
#endif
        }
    }
    else
    {
        MotStepper.eu8Dir = STPM_GO_UP;
        encoder_pulse = Charactor.saStep[seq].u16Val;
#ifdef SEND_LOG
        SendLogStr("Go continue\n");
        logLen = (uint8_t)sprintf(log1, "encoder_pulse = %d\n", encoder_pulse);
        SendLogStr(log1);
#endif
    }

    STPM_Run(MotStepper.eu8Dir, encoder_pulse);
    HAL_Delay(2);
}

void Start_Sawn(void)
{
    Cylinder_Write(&Cylinder[CY_HOLD_BILLET], CY_ON);
	HAL_Delay(500);
	Cylinder_Sawn();
	Cylinder_Write(&Cylinder[CY_HOLD_BILLET], CY_OFF);
}

void Get_Billet1st(void)
{
    if (true == State.bits.S_GET_BILLET)
    {
#ifdef SEND_LOG
        SendLogStr("S_GET_BILLET\n");
#endif
        CLCD_I2C_Clear(&LCD1);
        CLCD_I2C_SetCursor(&LCD1, 0, 0);
        CLCD_I2C_WriteString(&LCD1,"S_GET_BILLET");
        MotSawn_Write(MOT_ON);

		while (OPTICAL_OFF == OpSensor_ReadState(&OpSensor[OPTICAL_2])) {;}
		Start_Sawn();

		State.bits.S_GET_BILLET = false;
        State.bits.S_RUN_MACHINE = true;
    }
}



#ifdef SEND_LOG
void Log_Sawn(uint16_t step)
{
    logLen = (uint8_t)sprintf(log1, "RunInf.u16SeqLine = %d\n", RunInf.u16SeqLine);
    SendLogStr(log1);
    logLen = (uint8_t)sprintf(log1, "RunInf.u16SeqStepOfLine = %d\n", RunInf.u16SeqStepOfLine);
    SendLogStr(log1);
    logLen = (uint8_t)sprintf(log1, "Charactor.u16NumStep = %d\n", Charactor.u16NumStep);
    SendLogStr(log1);
    logLen = (uint8_t)sprintf(log1, "Charactor.saStep[i].u16Val = %d\n", Charactor.saStep[step].u16Val);
    SendLogStr(log1);
    logLen = (uint8_t)sprintf(log1, "Charactor.u16NumLine = %d\n", Charactor.u16NumLine);
    SendLogStr(log1);
    logLen = (uint8_t)sprintf(log1, "Charactor.sStep.u16SeqLine = %d\n", Charactor.saStep[step].u16SeqLine);
    SendLogStr(log1);
    logLen = (uint8_t)sprintf(log1, "Charactor.saStep.u16SeqStepOfLine = %d\n", Charactor.saStep[step].u16SeqStepOfLine);
    SendLogStr(log1);
    logLen = (uint8_t)sprintf(log1, "Charactor.saStep.eMode = %d\n", Charactor.saStep[step].eMode);
    SendLogStr(log1);
    logLen = (uint8_t)sprintf(log1, "Charactor.u16numStep = %d\n", step);
    SendLogStr(log1);
}
#endif

static void Sawn_Billet(void)
{
	uint16_t step = 0;
	while(true == State.bits.S_RUN_MACHINE)
	{
#ifdef SEND_LOG
		SendLogStr("RUNNING\n");
		logLen = (uint8_t)sprintf(log1, "RunInf.u16seqStep = %d\n", RunInf.u16SeqStep);
		SendLogStr(log1);
		logLen = (uint8_t)sprintf(log1, "Charactor.u16numStep = %d\n", Charactor.u16NumStep);
		SendLogStr(log1);
#endif
        UART_enableProcess = false;
//        BTN_bEnableProcess = false;
		for (step = RunInf.u16SeqStep; step < Charactor.u16NumStep; step++)
		{
            RunInf.u16SeqLine = Charactor.saStep[step].u16SeqLine;
            RunInf.u16SeqStepOfLine = Charactor.saStep[step].u16SeqStepOfLine;
#ifdef SEND_LOG
            Log_Sawn(step);
#endif
			HAL_Delay(2);
            PullBillet(step);
            HAL_Delay(100);
            Cylinder_Write(&Cylinder[CY_HOLD_BILLET], CY_ON);
            if (SAWN == Charactor.saStep[step].eMode)
            {
                Cylinder_Sawn();
                RunInf.bPreviousIsCut = false;
            }
            else
            {
                Cylinder_CutEnd();
                RunInf.bPreviousIsCut = true;
            }
            Cylinder_Write(&Cylinder[CY_HOLD_BILLET], CY_OFF);
#ifdef SEND_LOG
			logLen = (uint8_t)sprintf(log1, "Charactor.saStep[i].u16Val = %d\n", Charactor.saStep[step].u16Val);
			SendLogStr(log1);
#endif
            SendResponseToPC(CMD_RUNNING, &UARTSend);
            UART_enableProcess = true;
//            BTN_bEnableProcess = true;
//			if (S_RUN_MACHINE == M_OldState)
//			{
//                RunInf.u16SeqStep = step + (uint16_t)1;
//#ifdef SEND_LOG
//                SendLog("BREAK S_RUN_MACHINE\n");
//#endif
//                M_State = S_PROCESS_PC_DATA;
//				break;
//			}
		}
		break;
	}
    if (true == State.bits.S_RUN_MACHINE)
    {
#ifdef SEND_LOG
        SendLogStr("FINISH S_RUN_MACHINE\n");
#endif
//        CutMotorRun(false);
//        STPM_Enable(STPM_DISABLE);
        //M_State = S_STAND_BY;
    }

}

void RunMachine_Process(void)
{
    if (true == State.bits.S_RUN_MACHINE)
    {
#ifdef SEND_LOG
        SendLogStr("RunMachine_Process\n");
#endif
        CLCD_I2C_Clear(&LCD1);
        CLCD_I2C_SetCursor(&LCD1, 0, 0);
        CLCD_I2C_WriteString(&LCD1,"RUNNING--->");

        Sawn_Billet();
        CLCD_I2C_Clear(&LCD1);
        CLCD_I2C_SetCursor(&LCD1, 0, 0);
        CLCD_I2C_WriteString(&LCD1,"S_STAND_BY");
		MotSawn_Write(MOT_OFF);
        State.bits.S_RUN_MACHINE = false;
    }
}