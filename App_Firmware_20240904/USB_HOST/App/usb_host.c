/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file            : usb_host.c
  * @version         : v1.0_Cube
  * @brief           : This file implements the USB Host
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
  /* USER CODE END Header */

  /* Includes ------------------------------------------------------------------*/

#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "usb_process.h"
#include "File_Handling.h"
#include "main.h"
#include "log.h"
#include "watchdog.h"
#include "ff.h"
#include "usb_process.h"
#include "app.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Host core handle declaration */
USBH_HandleTypeDef hUsbHostFS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

FATFS USBH_fatfs;
FIL MyFile;
USB_MountType USB_Mount;
extern char USBHPath[];  /* USBH logical drive path */



/*
 * -- Insert your variables declaration here --
 */
 /* USER CODE BEGIN 0 */

 /* USER CODE END 0 */

 /*
  * user callback declaration
  */

static USB_MountType userFunction(void)
{
	FRESULT res = FR_OK;
	uint32_t bytesread = 0;
	USB_MountType recOK = U_UNMOUNT;
	const char file_name[10] = "app.txt";//name of the file

#ifdef SEND_LOG
	logLen = (uint8_t)sprintf(log1, "Start f_open = %d\n", USB_wait_ms);
	SendLogStr(log1);
#endif
    /*open file to verification*/
	if (f_open(&MyFile, file_name, FA_READ) != FR_OK)
	{
		/*file open failure*/
#ifdef SEND_LOG
        SendLogStr("FAIL OPEN!!!!\n");
#endif
		recOK = U_NO_FILE;
	}
	else
	{
        memset(&File, 0, sizeof(File));
#ifdef SEND_LOG	
		logLen = (uint8_t)sprintf(log1, "Start f_read = %d\n", USB_wait_ms);
		SendLogStr(log1);
#endif		
		res = f_read(&MyFile, &File.au8Data, USB_DATA_MAX_U16, &bytesread);
        File.u16Len = (uint16_t)bytesread;
#ifdef SEND_LOG
		logLen = (uint8_t)sprintf(log1, "Stop f_read = %d\n", USB_wait_ms);
		SendLogStr(log1);
#endif
		if (res != FR_OK)
		{
			/*read fail*/
#ifdef SEND_LOG
            SendLogStr("FAIL READ!!!!\n");
#endif
			recOK = U_NO_FILE;
		}
		else
		{
#ifdef SEND_LOG
            SendLogStr("START READ!!!!\n");
#endif
			if((USB_DATA_MAX_U16 - (uint32_t)1) < (uint16_t)bytesread)
			{
#ifdef SEND_LOG
                SendLogStr("LARGE DATA!!!!\n");
#endif
				recOK = U_INVALID_DATA;
			}
			else
			{
#ifdef SEND_LOG
                logLen = (uint8_t)sprintf(log1, "Size Of Text = %d\n", bytesread);
				SendLogStr(log1);
				SendLogStr("WHILE READ!!!!\n");
#endif
				File.u16Len = (uint16_t)bytesread;
				recOK = U_VALID_DATA;
			}
		}
        f_close(&MyFile);
	}
    
	USB_Mount = recOK;
	return recOK;
}

static void USBH_UserProcess(USBH_HandleTypeDef* phost, uint8_t id)
{
	/* USER CODE BEGIN CALL_BACK_1 */
	switch (id)
	{
	case HOST_USER_SELECT_CONFIGURATION:
#ifdef SEND_LOG
        SendLogStr("\nHOST_USER_SELECT_CONFIGURATION\n");
#endif
		break;

	case HOST_USER_DISCONNECTION:
#ifdef SEND_LOG
        SendLogStr("\nHOST_USER_DISCONNECTION\n");
#endif
		Appli_state = APPLICATION_DISCONNECT;
		break;

	case HOST_USER_CLASS_ACTIVE:
#ifdef SEND_LOG
        SendLogStr("\nHOST_USER_CLASS_ACTIVE\n");
#endif
		Appli_state = APPLICATION_READY;
		break;

	case HOST_USER_CONNECTION:
#ifdef SEND_LOG
        SendLogStr("\nHOST_USER_CONNECTION\n");
#endif
		Appli_state = APPLICATION_START;
		if (f_mount(&USBH_fatfs, USBHPath, 0) != FR_OK)
		{
#ifdef SEND_LOG
             SendLogStr("MOUNT OK MOUNT OK\n");
#endif
            ;
		}
		break;

	default:
#ifdef SEND_LOG
        SendLogStr("\nDEFAULT\n");
#endif
		break;
	}
	/* USER CODE END CALL_BACK_1 */
}
/*
 * -- Insert your external function declaration here --
 */
 /* USER CODE BEGIN 1 */

 /* USER CODE END 1 */

 /**
   * Init USB host library, add supported class and start the library
   * @retval None
   */
void MX_USB_HOST_Init(void)
{
	/* USER CODE BEGIN USB_HOST_Init_PreTreatment */

	/* USER CODE END USB_HOST_Init_PreTreatment */

	/* Init host Library, add supported class and start the library. */
	if (USBH_Init(&hUsbHostFS, USBH_UserProcess, HOST_FS) != USBH_OK)
	{
		Error_Handler();
	}
	if (USBH_RegisterClass(&hUsbHostFS, USBH_MSC_CLASS) != USBH_OK)
	{
		Error_Handler();
	}
	if (USBH_Start(&hUsbHostFS) != USBH_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USB_HOST_Init_PostTreatment */

	/* USER CODE END USB_HOST_Init_PostTreatment */
}




/*
 * Background task
 */
USB_MountType MX_USB_HOST_Process(void)
{
	USB_MountType recOK = U_UNMOUNT;
#ifdef SEND_LOG
	SendLogStr("Continue Continue\n");
#endif
	/* USB Host Background task */
	
	Appli_state = APPLICATION_IDLE;
    MX_FATFS_Init();
	MX_USB_HOST_Init();
    USB_wait_ms = 0;
    memset(&File, 0, sizeof(File));
	while (MAX_USB_WAIT_MS > USB_wait_ms)
	{
		USBH_Process(&hUsbHostFS);
//		logLen = sprintf(log, "Wait USBH_Process = %d\n", USB_wait_ms);
//		SendLog(LOG_USB, (uint8_t*)log, (uint16_t)logLen);
		if (APPLICATION_READY == Appli_state)
		{
#ifdef SEND_LOG
			SendLogStr("APPLICATION_READY!!!!\n");
#endif
			recOK = userFunction();
			if ((U_VALID_DATA == recOK)||(U_INVALID_DATA == recOK))
			{
				break;
			}
		}
		if (MAX_USB_WAIT_MS <= USB_wait_ms)
		{
			break;
		}
	}
	return recOK;
}

/*
 * user callback definition
 */


/**
  * @}
  */

  /**
	* @}
	*/

	/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


