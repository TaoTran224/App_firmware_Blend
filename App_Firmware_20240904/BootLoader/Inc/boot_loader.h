
#ifndef BOOT_LOADER_H
#define BOOT_LOADER_H
#include "main.h"
#include "flash.h"
//#define FW_APPLICATION
//#define FW_BOOT_LOADER

#define ADDR_FW_APP_START 0x08020000
//#define ADDR_REQUEST_PROG_FW 0x08010000
#define ADDR_PAGE_REQUEST_PROG_FW 64
//#define ADDR_FW_BOOT_VERSION 0x08010400
#define ADDR_PAGE_FW_BOOT_VERSION 65
#define PAGE_BOOT_LOADER 128
#define FLAG_PROG_FW_LEN 16
#define FW_VERSION_LEN 20

extern char FW_version[FW_VERSION_LEN];
extern uint8_t FW_au8buf[FLASH_BLOCK_SIZE];
extern uint16_t FW_u16RealAddrPage; // dinh danh dia chi page
extern uint16_t FW_u16SeqPage;  // so thu tu page, page cuoi cung =  FW_u16NumPage - 1
extern uint16_t FW_u16NumPage; // tong so page

void BL_ResetRequestProgFW(void);
void BL_WriteRequestProgFW(void);
bool BL_ReadRequestProgFW(void);
void BL_GotoApp(void);

#endif