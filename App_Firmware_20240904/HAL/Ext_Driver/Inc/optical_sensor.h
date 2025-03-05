#ifndef OPTICAL_SENSOR_H_
#define OPTICAL_SENSOR_H_
#include "main.h"

#define OPSEN_MAX_NUM (uint8_t)2

#define OPTICAL_1 (uint8_t)0
#define OPTICAL_2 (uint8_t)1


typedef enum {
OPTICAL_1_DEF = (uint8_t)21,
OPTICAL_2_DEF = (uint8_t)22,

} OpticalNameType;

typedef enum {
    OPTICAL_ON = (uint8_t)0,
    OPTICAL_OFF = (uint8_t)1,
} OpticalStateType;

typedef struct{
    OpticalNameType eu8Name;
    OpticalStateType eu8State;
}OpSensorType;

extern OpSensorType OpSensor[OPSEN_MAX_NUM];

void OpSensor_Init(void);
OpticalStateType OpSensor_ReadState(OpSensorType* OpSensor);
#endif
