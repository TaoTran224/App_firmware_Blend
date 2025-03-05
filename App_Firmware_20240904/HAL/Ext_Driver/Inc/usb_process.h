/*
 * usb_process.h
 *
 *  Created on: Jan 15, 2022
 *      Author: TRAN NHU TAO
 */

#ifndef __USB_PROCESS_H_
#define __USB_PROCESS_H_

#include "main.h"

//#define USART_DATA_MAX (uint32_t) 0x5401
#define USB_DATA_MAX_U16 (uint16_t) 0x5000 //

typedef enum 
{
    CHECK_NEVER = (uint8_t)0,
    CHECK_OK = (uint8_t)0x06,
    CHECK_FAIL = (uint8_t)0x15,
} FlagCheckFile;

typedef enum
{
    BLOCK_OK = (uint8_t)0x06,
    BLOCK_FAIL = (uint8_t)0x15,
    BLOCK_FILE_OK = (uint8_t)0x54,
    BLOCK_FILE_FAIL = (uint8_t)0x46,
} FlagRealBLock;

typedef struct
{
	uint8_t au8Data[USB_DATA_MAX_U16];
	uint16_t u16RealLen;
    uint16_t u16Len;
    uint16_t u16NumFrame;
    uint16_t u16RealSeqOfFrame;
    uint16_t u16PreviousSeqOfFrame;
    FlagRealBLock eIsBlockChecked;
    FlagCheckFile eIsFileChecked;
}__attribute__((packed)) FileType;

extern FileType File;

bool GetFileFromUSB(void);

#endif /* INC_USB_PROCESS_H_ */
