#ifndef FLASH_H
#define FLASH_H
#include "stdint.h"
#include "string.h"
#include "main.h"

#define FLASH_BLOCK_SIZE 1024

//#define STM32F103C8
//#ifdef STM32F103C8
//
//#define PAGE_START 20
//#define PAGE_STOP 64
//#endif


#define STM32F407
#ifdef STM32F407
#define PAGE_START 64
#define PAGE_STOP 511
#endif
//#define STM32F407VG
//#ifdef STM32F407VG
//#define PAGE_START 32
//#define PAGE_STOP 61
//#endif

#define FL_SECTOR_FLAG FLASH_SECTOR_4
#define FL_SECTOR_FW_APP FLASH_SECTOR_5
void FL_EraseSector(uint8_t page);

void FL_WritePage(uint16_t page, uint8_t* buf, uint16_t size);
void FL_ReadPage(const uint16_t page, uint8_t* buf, uint16_t size);

#endif