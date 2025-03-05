#ifndef CYLINDER_H_
#define CYLINDER_H_
#include "main.h"

#define CY_MAX_NUM (uint8_t)4

#define CY_SAWN_BILLET (uint8_t)0
#define CY_PULL_BILLET (uint8_t)1
#define CY_HOLD_BILLET (uint8_t)3
#define CY_CUT_END (uint8_t)2

typedef enum {

CY_SAWN_BILLET_DEF = (uint8_t)0,
CY_PULL_BILLET_DEF = (uint8_t)1,
CY_CUT_END_DEF = (uint8_t)2,
CY_HOLD_BILLET_DEF = (uint8_t)3
} CylinderNameType;

typedef enum {
    CY_ON = (uint8_t)0,
    CY_OFF = (uint8_t)1,
} CylinderModeType;


typedef struct{
    CylinderNameType eu8Name;
    CylinderModeType eu8Mode;
} __attribute__((packed)) CylinderType;

extern CylinderType Cylinder[CY_MAX_NUM];

void Cylinder_Init(void);
void Cylinder_Write(CylinderType* cyl, const CylinderModeType mode);

void Cylinder_Sawn(void);
void Cylinder_CutEnd(void);

#endif