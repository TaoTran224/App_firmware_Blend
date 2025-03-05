
#include "watchdog.h"
#include "board.h"
#include "iwdg.h"

void WDT_Clear(void)
{
	HAL_IWDG_Refresh(&hiwdg);
}
