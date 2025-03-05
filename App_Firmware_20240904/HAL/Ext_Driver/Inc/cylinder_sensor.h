#ifndef CYLINDER_SENSOR_H_
#define CYLINDER_SENSOR_H_
#include "main.h"

#define CYSEN_MAX_NUM (uint8_t)5

#define CYSEN_PULL_BILLET (uint8_t)0
#define CYSEN_SAWN_OFF (uint8_t)1
#define CYSEN_SAWN_ON (uint8_t)2
#define CYSEN_HOLD_BILLET (uint8_t)3
#define CYSEN_CUT_END (uint8_t)4

// sensor cylider (input)
typedef enum {
CYSEN_PULL_BILLET_DEF = (uint8_t)18,
CYSEN_SAWN_OFF_DEF = (uint8_t)17,
CYSEN_SAWN_ON_DEF = (uint8_t)11,
CYSEN_HOLD_BILLET_DEF = (uint8_t)19,
CYSEN_CUT_END_DEF = (uint8_t)20
} CySensorNameType;

typedef enum {
    CYSEN_ON = (uint8_t)0,
    CYSEN_OFF = (uint8_t)1,
} CySensorStateType;

typedef struct{
    CySensorNameType eu8Name;
    CySensorStateType eu8State;
} __attribute__((packed)) CySensorType;

extern CySensorType CySensor[CYSEN_MAX_NUM];
CySensorStateType CySensor_ReadState(CySensorType* cysen);
void CySensor_Init(void);


#endif
