#include "boot_loader.h"
#include "main.h"
#include "log.h"

#ifdef FW_BOOT_LOADER
	char FW_version[FW_VERSION_LEN] = "V1.0";
#else
	char FW_version[FW_VERSION_LEN] = "V1.0.20240804";
#endif

uint8_t FW_au8buf[FLASH_BLOCK_SIZE];
uint16_t FW_u16RealAddrPage;
uint16_t FW_u16SeqPage;
uint16_t FW_u16NumPage;

static void BL_WriteProgFlag(uint8_t* flag, uint16_t len)
{
	uint8_t ver[FW_VERSION_LEN];
	uint8_t buf[FLAG_PROG_FW_LEN + FW_VERSION_LEN];

	memset(ver, 0, sizeof(ver));
#ifdef FW_BOOT_LOADER
	memcpy(ver, (uint8_t*)FW_version, 4);
#endif
	memset(buf, 0, sizeof(buf));

	FL_ReadPage(ADDR_PAGE_REQUEST_PROG_FW, buf, sizeof(buf));
	memcpy(ver, buf + len, sizeof(ver));
#ifdef SEND_LOG
    SendLog(buf, sizeof(buf));
#endif
	memcpy(buf, flag, len);
	memcpy(buf + len, ver, sizeof(ver));
#ifdef SEND_LOG
    SendLog(buf, sizeof(buf));
#endif
	FL_EraseSector(FL_SECTOR_FLAG);
	FL_WritePage(ADDR_PAGE_REQUEST_PROG_FW, buf, sizeof(buf));
}

void BL_ResetRequestProgFW(void)
{
    uint8_t Flag_au8BootLoader[FLAG_PROG_FW_LEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
#ifdef SEND_LOG
        SendLogStr("BL_ResetRequestProgFW\n");
#endif
	BL_WriteProgFlag(Flag_au8BootLoader, sizeof(Flag_au8BootLoader));
}

void BL_WriteRequestProgFW(void)
{
    uint8_t Flag_au8BootLoader[FLAG_PROG_FW_LEN] = {0x23, 0xC6, 0x4B, 0x0C, 0x29, 0xEA, 0xF7, 0x9D, 0xB0, 0xA5, 0x81, 0x91, 0xA4, 0x8F, 0x5C, 0x7F};
#ifdef SEND_LOG
        SendLogStr("BL_WriteRequestProgFW\n");
#endif
	BL_WriteProgFlag(Flag_au8BootLoader, sizeof(Flag_au8BootLoader));
}

bool BL_ReadRequestProgFW(void)
{
    uint8_t Flag_au8BootLoader[FLAG_PROG_FW_LEN] = {0x23, 0xC6, 0x4B, 0x0C, 0x29, 0xEA, 0xF7, 0x9D, 0xB0, 0xA5, 0x81, 0x91, 0xA4, 0x8F, 0x5C, 0x7F};
	uint8_t buf[FLAG_PROG_FW_LEN + FW_VERSION_LEN];
	FL_ReadPage(ADDR_PAGE_REQUEST_PROG_FW, buf, sizeof(buf));
#ifdef SEND_LOG
	SendLogStr("BL_ReadRequestProgFW\n");
	SendLog(buf, sizeof(buf));
#endif
	if (0 == memcmp(buf, Flag_au8BootLoader, sizeof(Flag_au8BootLoader)))
	{
#ifdef SEND_LOG
        SendLogStr("Has Request Prog FW\n");
#endif
		return true;
	}
	return false;
}


void BL_GotoApp(void)
{
	void (*SysMemBootJump)(void);
	volatile uint32_t addr = ADDR_FW_APP_START;
#ifdef SEND_LOG
        SendLogStr("BL_GotoApp\n");
#endif
    #if defined(USE_HAL_DRIVER)
	HAL_RCC_DeInit();
	#endif /* defined(USE_HAL_DRIVER) */
	#if defined(USE_STDPERIPH_DRIVER)
			RCC_DeInit();
	#endif /* defined(USE_STDPERIPH_DRIVER) */

	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;

	__disable_irq();
	#if defined(STM32F4)
	SYSCFG->MEMRMP = 0x01;
	#endif
	#if defined(STM32F0)
			SYSCFG->CFGR1 = 0x01;
	#endif

	SysMemBootJump = (void (*)(void)) (*((uint32_t *)(addr + 4)));

	__set_MSP(*(uint32_t *)addr);

	SysMemBootJump();
}
