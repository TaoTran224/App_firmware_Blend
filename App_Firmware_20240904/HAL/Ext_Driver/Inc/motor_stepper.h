
#ifndef MOTOR_STEPPER_H_
#define MOTOR_STEPPER_H_

#include "board.h"

typedef enum{
STPM_ENABLE=0,
STPM_DISABLE=1
}Enable_t;

typedef enum{
STPM_GO_UP=0,
STPM_GO_BACK=1
}Dir_t;

typedef struct{
    Enable_t eu8Enable;
    Dir_t eu8Dir;
}MotStepperType;

extern MotStepperType MotStepper;

void STPM_Init(void);

void STPM_Dir(const Dir_t dir);

void STPM_PulseStart(void);
void STPM_PulseStartPullBillet(void);
void STPM_PulseStop(void);

void STPM_Enable(const Enable_t enable);
void STPM_Run(const Dir_t dir, const uint32_t pulse);


#endif
