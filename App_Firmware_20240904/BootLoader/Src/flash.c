#include "flash.h"
#include "main.h"
#include "stm32f4xx_hal_flash.h"
#include "stm32f4xx_hal_flash_ex.h"

/*
Sector 0 0x0800 0000 - 0x0800 3FFF 16 Kbytes
Sector 1 0x0800 4000 - 0x0800 7FFF 16 Kbytes
Sector 2 0x0800 8000 - 0x0800 BFFF 16 Kbytes
Sector 3 0x0800 C000 - 0x0800 FFFF 16 Kbytes
Sector 4 0x0801 0000 - 0x0801 FFFF 64 Kbytes
Sector 5 0x0802 0000 - 0x0803 FFFF 128 Kbytes
Sector 6 0x0804 0000 - 0x0805 FFFF 128 Kbytes
.
Sector 11 0x080E 0000 - 0x080F FFFF 128 Kbytes

//#define FLASH_SECTOR_0     0U  !< Sector Number 0
//#define FLASH_SECTOR_1     1U  !< Sector Number 1

*/
void FL_EraseSector(uint8_t sector)
{

	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PageError = 0;

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector = sector;
	EraseInitStruct.NbSectors = 1;
	HAL_FLASH_Unlock();
     //__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR );
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
	{
		;
	}
	HAL_FLASH_Lock();

}

void FL_WriteAddr(uint32_t addr, uint8_t* buf, uint16_t len)
{
//	uint32_t page_addr =  0;
	uint16_t data;
	HAL_FLASH_Unlock();
	//__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR );
	for(uint16_t i=0;i<(len<<1);i++)
	{
		addr += (uint32_t)(i<<1);
		data=((uint16_t)buf[(i<<1) + 1]<<8) | ((uint16_t)buf[i<<1]);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr, data);
	}
	HAL_FLASH_Lock();
}

void FL_WritePage(uint16_t page, uint8_t* buf, uint16_t size)
{

	uint32_t page_addr = ((uint32_t)0x08000000 + (page * FLASH_BLOCK_SIZE)); //tinh dia chi cua page
	uint32_t addr;
	uint16_t data;

	if ((PAGE_START <= page) && (PAGE_STOP > page))
	{
		HAL_FLASH_Unlock();
		//__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR );
		for(uint16_t i=0;i<(size>>1);i++)
		{
			addr = page_addr + (i<<1);
			data=((uint16_t)buf[(i<<1)+1]<<8) | ((uint16_t)buf[(i<<1)]);
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr, data);
		}

		HAL_FLASH_Lock();
	}
}

uint16_t F_read(uint32_t addr)
{
	uint16_t* val = (uint16_t *)addr;
	return *val;
}

void FL_ReadPage(const uint16_t page, uint8_t* buf, uint16_t size)
{
	uint32_t page_addr = ((uint32_t)0x08000000 + (page * FLASH_BLOCK_SIZE));
	uint16_t val = 0;
	uint32_t addr;
	for (uint16_t i = 0; i < (size>>1); i++)
	{
		addr = page_addr + (i << 1);
		val = F_read(addr);
		buf[i<<1] = (uint8_t)val;
		buf[1 + (i<<1)] = (uint8_t)(val>>8);
	}
}


