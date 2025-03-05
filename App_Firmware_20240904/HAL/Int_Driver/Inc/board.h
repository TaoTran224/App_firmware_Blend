#ifndef __BOARD_H
#define __BOARD_H

#include "main.h"

//#include "stm32f4xx_hal.h"
#define INPUT_MAX (uint8_t)28
#define OUTPUT_MAX (uint8_t)23

// control motor
#define MOTOR_PULL_BILLET (uint8_t)14
#define MOTOR_SAWN (uint8_t)15
// ligths signal
typedef enum {
LS_BTN_START = (uint8_t)7,
LS_BTN_PAUSE = (uint8_t)8,
LS_BTN_STOP = (uint8_t)9 
} SignalLigthsType;




// sensor cylider (input)
typedef enum {
SO_PULL_BILLET_1 = (uint8_t)21,
SO_PULL_BILLET_2 = (uint8_t)22,
} Sensor_OpticalType;




typedef struct
{
    uint16_t Pinx;
    GPIO_TypeDef *GPIOx;
} __attribute__((packed)) InputInfType;

typedef struct
{
    uint16_t u16TimeNoise;
    bool bIsDetectNoise;
    bool bIsPress;
} __attribute__((packed)) InputType;


typedef enum
{
M_OFF = (uint8_t)0,
M_ON = (uint8_t)1,
M_BLINK = (uint8_t)2,
} OutputModeType;

typedef enum {
MOT_ON = (uint8_t)0,
MOT_OFF = (uint8_t)1
}MotModeType;

typedef struct
{
    uint16_t Pinx;
    GPIO_TypeDef *GPIOx;
    GPIO_PinState PinState;
    OutputModeType eu8Mode;
} __attribute__((packed)) OutputType;


extern InputInfType InputInf[INPUT_MAX];
extern InputType Input[INPUT_MAX];
extern OutputType Output[OUTPUT_MAX];


void delay_ms(uint32_t t);
void BoardInit(void);
void DebugOutput(void);

void HardwareInit(void);

void delay_us(uint32_t us);
#endif
