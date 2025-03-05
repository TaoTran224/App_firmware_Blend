#ifndef BUZZER_H_
#define BUZZER_H_
#include "main.h"

#define BUZZER_MAX_NUM (uint8_t)2

#define BUZZER_INT (uint8_t)0
#define BUZZER_EXT (uint8_t)1

typedef enum {
    BUZZER_ON = (uint8_t)0,
    BUZZER_OFF = (uint8_t)1,
} BuzzerModeType;

typedef enum{
    BUZZER_INT_DEF = (uint8_t)16,
    BUZZER_EXT_DEF = (uint8_t)6
} BuzzerNameType;

typedef struct{
    BuzzerNameType eu8Name;    
    BuzzerModeType eu8Mode;
} BuzzerType;

extern BuzzerType Buzzer[BUZZER_MAX_NUM];

void Buzzer_Init(void);

void Buzzer_Write(BuzzerType* buz, const BuzzerModeType mode);

void Buzzer_Process(void);
#endif
  