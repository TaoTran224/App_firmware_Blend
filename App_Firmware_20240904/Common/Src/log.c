
#include "log.h"
#include "main.h"
#include "board.h"
#include "serial.h"

#ifdef SEND_LOG
    char log1[50];
    uint8_t logLen;
void SendLog(const uint8_t* data, const uint16_t length)
{
    SendUART3(data, length);
}

void SendLogStr(const char* str)
{
    SendUART3((uint8_t*)str, strlen(str));
}
#endif
