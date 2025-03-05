
#ifndef __LOG_H_
#define __LOG_H_

#include "main.h"

typedef enum
{
	LOG_USB = 0, // only log serial
	LOG_RUN, // only PC serial
	LOG_DEBUG // all
} LogType;

#ifdef SEND_LOG
    extern char log1[50];
    extern uint8_t logLen;
void SendLog(const uint8_t* data, const uint16_t length);
void SendLogStr(const char* str);

#endif

#endif
