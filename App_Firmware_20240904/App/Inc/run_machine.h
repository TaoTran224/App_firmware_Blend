
#ifndef RUN_MACHINE_H_
#define RUN_MACHINE_H_
#include "process_data.h"


#define INT_NOISE_MAX_U16 (uint16_t)100
#define INT_TIMEOUT_MAX_U16 (uint16_t)300
#define UART_TIMEOUT_U8 (uint8_t)200

extern CharactorType Charactor;


typedef struct
{
	uint16_t u16SeqLine; // so thu tu duong hien tai
    uint16_t u16SeqStepOfLine; // so thu tu doan hien tai trong line
    uint16_t u16SeqStep; // so thu tu buoc hien tai trong file
//	uint16_t u16EncoderPulse; //dem so xung encoder hien tai
//	bool bEnable;
    bool bPreviousIsCut;
//    bool bDir;
    FlagCutType eIsCutChecked;
}__attribute__((packed)) RunInfType;

typedef struct
{
    double d64d2;
//    double d1;
//    uint16_t puly2;
//    uint16_t puly1;
    double d64DistanceSwanToCut;
//    uint16_t encoderPulseNum;
//    uint16_t stepperPulseNum;
} __attribute__((packed)) MachineSettingType;

extern RunInfType RunInf;
extern UARTRecType UARTRec;
extern MachineSettingType MachineSetting;

void Get_Billet1st(void);
void RunMachine_Process(void);
#endif
