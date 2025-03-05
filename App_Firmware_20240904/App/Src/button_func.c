#include "button_func.h"
#include "button.h"
#include "motor_stepper.h"
#include "motor_sawn.h"
#include "motor_billet.h"
#include "cylinder.h"
#include "log.h"
void Button_Init(void)
{
    memset(Button, 0, sizeof(Button));
    Button[BTN_START].eu8Name = BTN_START_DEF;
    Button[BTN_PAUSE].eu8Name = BTN_PAUSE_DEF;
    Button[BTN_STOP].eu8Name = BTN_STOP_DEF;
    Button[BTN_BILLET_GO_UP].eu8Name = BTN_BILLET_GO_UP_DEF;
    Button[BTN_MOTOR_SAWN].eu8Name = BTN_MOTOR_SAWN_DEF;
    Button[BTN_CYLINDER_SAWN].eu8Name = BTN_CYLINDER_SAWN_DEF;
    Button[BTN_BILLET_GO_BACK].eu8Name = BTN_BILLET_GO_BACK_DEF;
    Button[BTN_CYLINDER_HOLD_BILLET].eu8Name = BTN_CYLINDER_HOLD_BILLET_DEF;
    Button[BTN_CYLINDER_CUT].eu8Name = BTN_CYLINDER_CUT_DEF;
}

//
//#define BTN_BILLET_GO_DOWN (uint8_t)6
//#define BTN_CYLINDER_SAWN (uint8_t)7
//#define BTN_CYLINDER_CUT (uint8_t)8
//#define BTN_EMERGENCY (uint8_t)9

void Button_Detect(void)
{
    BTN_DetectPin(&Button[BTN_START], 5);
    BTN_DetectPin(&Button[BTN_PAUSE], 5);
    BTN_DetectPin(&Button[BTN_STOP], 5);
    BTN_DetectPin(&Button[BTN_BILLET_GO_UP], 5);
    BTN_DetectPin(&Button[BTN_MOTOR_SAWN], 5);
    BTN_DetectPin(&Button[BTN_CYLINDER_SAWN], 5);
    BTN_DetectPin(&Button[BTN_BILLET_GO_BACK], 5);
    BTN_DetectPin(&Button[BTN_CYLINDER_HOLD_BILLET], 5);
    BTN_DetectPin(&Button[BTN_CYLINDER_CUT], 5);
}

void BTN_StartProcess(void)
{
    if (true == Button[BTN_START].bIsPress)
    {
#ifdef SEND_LOG
        SendLogStr("BTN_START Process\n");
#endif
        BTN_ClearPin(&Button[BTN_START]);
    }
}

void BTN_PauseProcess(void)
{
    if (true == Button[BTN_PAUSE].bIsPress)
    {
#ifdef SEND_LOG
        SendLogStr("BTN_PAUSE Process\n");
#endif
        BTN_ClearPin(&Button[BTN_PAUSE]);
    }
}

void BTN_StopProcess(void)
{
    if (true == Button[BTN_STOP].bIsPress)
    {
#ifdef SEND_LOG
        SendLogStr("BTN_STOP Process\n");
#endif
        BTN_ClearPin(&Button[BTN_STOP]);
    }
}

void BTN_BilletGoUpProcess(void)
{
    if (true == Button[BTN_BILLET_GO_UP].bIsPress)
    {
#ifdef SEND_LOG
        SendLogStr("BTN_BILLET_GO_UP Process\n");
#endif
        Cylinder_Write(&Cylinder[CY_PULL_BILLET], CY_OFF);
        Cylinder_Write(&Cylinder[CY_HOLD_BILLET], CY_OFF);
        //Cylinder_Write(&Cylinder[CY_SAWN_BILLET], CY_OFF);
        Cylinder_Write(&Cylinder[CY_CUT_END], CY_OFF);
        STPM_Dir(STPM_GO_UP);
        STPM_PulseStartPullBillet();
        STPM_Enable(STPM_ENABLE);
        while (true == BTN_IsPress(&Button[BTN_BILLET_GO_UP]));
        STPM_Enable(STPM_DISABLE);
        STPM_PulseStop();
        BTN_ClearPin(&Button[BTN_BILLET_GO_UP]);
    }
}

void BTN_BilletGoBackProcess(void)
{
    if (true == Button[BTN_BILLET_GO_BACK].bIsPress)
    {
#ifdef SEND_LOG
        SendLogStr("BTN_BILLET_GO_BACK Process\n");
#endif
        Cylinder_Write(&Cylinder[CY_PULL_BILLET], CY_OFF);
        Cylinder_Write(&Cylinder[CY_HOLD_BILLET], CY_OFF);
        Cylinder_Write(&Cylinder[CY_SAWN_BILLET], CY_OFF);
        Cylinder_Write(&Cylinder[CY_CUT_END], CY_OFF);
        STPM_Dir(STPM_GO_BACK);
        STPM_PulseStartPullBillet();
        STPM_Enable(STPM_ENABLE);
        while (true == BTN_IsPress(&Button[BTN_BILLET_GO_BACK]));
        STPM_Enable(STPM_DISABLE);
        STPM_PulseStop();
        BTN_ClearPin(&Button[BTN_BILLET_GO_BACK]);
    }
}

void BTN_MotorSawnProcess(void)
{
    if (true == Button[BTN_MOTOR_SAWN].bIsPress)
    {
#ifdef SEND_LOG
        SendLogStr("BTN_MOTOR_SAWN Process\n");
#endif
        if (1 == ((++Button[BTN_MOTOR_SAWN].u8TimesPress) & 0x01))
        {
            MotSawn_Write(MOT_OFF);
        }
        else
        {
            MotSawn_Write(MOT_ON);
        }
        delay_ms(100);
        BTN_ClearPin(&Button[BTN_MOTOR_SAWN]);
    }
}

void BTN_CylinderSawnProcess(void)
{
    if (true == Button[BTN_CYLINDER_SAWN].bIsPress)
    {
#ifdef SEND_LOG
        SendLogStr("BTN_CYLINDER_SAWN Process\n");
#endif
        if (1 == ((++Button[BTN_CYLINDER_SAWN].u8TimesPress) & 0x01))
        {
            Cylinder_Write(&Cylinder[CY_SAWN_BILLET], CY_ON);
        }
        else
        {
            Cylinder_Write(&Cylinder[CY_SAWN_BILLET], CY_OFF);
        }
        delay_ms(100);
        BTN_ClearPin(&Button[BTN_CYLINDER_SAWN]);
    }
}

void BTN_CylinderHoldBilletProcess(void)
{
    if (true == Button[BTN_CYLINDER_HOLD_BILLET].bIsPress)
    {
#ifdef SEND_LOG
        SendLogStr("BTN_CYLINDER_HOLD_BILLET Process\n");
#endif
        if (1 == ((++Button[BTN_CYLINDER_HOLD_BILLET].u8TimesPress) & 0x01))
        {
            Cylinder_Write(&Cylinder[CY_HOLD_BILLET], CY_ON);
        }
        else
        {
            Cylinder_Write(&Cylinder[CY_HOLD_BILLET], CY_OFF);
        }
        delay_ms(100);
        BTN_ClearPin(&Button[BTN_CYLINDER_HOLD_BILLET]);
    }
}

void BTN_CylinderCutProcess(void)
{
    if (true == Button[BTN_CYLINDER_CUT].bIsPress)
    {
#ifdef SEND_LOG
        SendLogStr("BTN_CYLINDER_HOLD_BILLET Process\n");
#endif
        if (1 == ((++Button[BTN_CYLINDER_CUT].u8TimesPress) & 0x01))
        {
            Cylinder_Write(&Cylinder[CY_CUT_END], CY_ON);
        }
        else
        {
            Cylinder_Write(&Cylinder[CY_CUT_END], CY_OFF);
        }
        delay_ms(100);
        BTN_ClearPin(&Button[BTN_CYLINDER_CUT]);
    }
}

void Button_Process(void)
{
    BTN_StartProcess();
    BTN_PauseProcess();
    BTN_StopProcess();
    BTN_BilletGoUpProcess();
    BTN_BilletGoBackProcess();
    BTN_MotorSawnProcess();
    BTN_CylinderSawnProcess();
    BTN_CylinderHoldBilletProcess();
    BTN_CylinderCutProcess();
}