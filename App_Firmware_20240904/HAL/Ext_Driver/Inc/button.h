/*
 * button.h
 *
 *  Created on: Feb 26, 2022
 *      Author: TRAN NHU TAO
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "interrupt_process.h"

#define BTN_MAX_NUM (uint8_t)(28)
   
#define BTN_START (uint8_t)0
#define BTN_PAUSE (uint8_t)1
#define BTN_STOP (uint8_t)2

#define BTN_BILLET_GO_BACK (uint8_t)3
#define BTN_MOTOR_SAWN (uint8_t)4
#define BTN_CYLINDER_HOLD_BILLET (uint8_t)5//xx

#define BTN_BILLET_GO_UP (uint8_t)6
#define BTN_CYLINDER_SAWN (uint8_t)7
#define BTN_CYLINDER_CUT (uint8_t)9
#define BTN_EMERGENCY (uint8_t)8

#define BTN_USB1 (uint8_t)10
#define BTN_USB2 (uint8_t)11
#define BTN_USB3 (uint8_t)12
#define BTN_USB4 (uint8_t)13

typedef enum {
BTN_START_DEF = (uint8_t)0,
BTN_PAUSE_DEF = (uint8_t)1,
BTN_STOP_DEF = (uint8_t)2,

BTN_BILLET_GO_BACK_DEF = (uint8_t)3,
BTN_MOTOR_SAWN_DEF = (uint8_t)4,
BTN_CYLINDER_HOLD_BILLET_DEF = (uint8_t)5,

BTN_BILLET_GO_UP_DEF = (uint8_t)6,
BTN_CYLINDER_SAWN_DEF = (uint8_t)7,
BTN_CYLINDER_CUT_DEF = (uint8_t)9,

BTN_EMERGENCY_DEF = (uint8_t)8,

BTN_USB1_DEF = (uint8_t)10,
BTN_USB2_DEF = (uint8_t)14,
BTN_USB3_DEF = (uint8_t)15,
BTN_USB4_DEF = (uint8_t)16,

} BTN_NameType;

typedef struct 
{
    BTN_NameType eu8Name;
    uint16_t u16TimeNoise;
    bool bIsDetectNoise;
    bool bIsPress;
    uint8_t u8TimesPress;
} __attribute__((packed)) ButtonType;

extern ButtonType Button[BTN_MAX_NUM];

void BTN_ClearPin(ButtonType* btn);
void BTN_DetectPin(ButtonType* btn, uint16_t timeout);
bool BTN_IsPress(const ButtonType* btn);

#endif /* INC_BUTTON_H_ */
