
#include <usb_process.h>
#include "log.h"
#include "watchdog.h"
#include "usb_host.h"

FileType File;

bool GetFileFromUSB(void)
{
	//uint8_t log[40];
	//uint8_t logLen = 0;
	//bool processOK = false;
	if (U_VALID_DATA == MX_USB_HOST_Process())
	{
#ifdef SEND_LOG
		SendLogStr("GetFileFromUSB\n");
#endif
		return true;
	}
	return false;
}




