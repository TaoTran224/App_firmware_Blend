
#ifndef APP_H_
#define APP_H_

#include "main.h"


/*
typedef struct
{
	uint8_t i1[20];
	uint16_t i2[10];
	uint32_t i3[30];
}at;

typedef struct
{
	uint8_t i1[20];
	uint16_t i2[10];
	uint32_t i3[30];
}__attribute__((packed)) bt;

extern at t1;
extern bt t2;
extern uint8_t a1, a2;
*/


void StartUp(void);
void Loop(void);
void Loop_Debug(void);


#endif
