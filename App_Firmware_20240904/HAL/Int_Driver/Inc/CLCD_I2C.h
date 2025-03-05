#ifndef __CLCD_I2C_H
#define __CLCD_I2C_H

#include "stm32f4xx_hal.h"

#define CLCD_COMMAND 	(uint8_t)0x00
#define CLCD_DATA 		(uint8_t)0x01

#define LCD_EN (uint8_t)0x04  // Enable bit
#define LCD_RW (uint8_t)0x02  // Read/Write bit
#define LCD_RS (uint8_t)0x01  // Register select bit

#define CLCD_COMMAND 			(uint8_t)0x00
#define CLCD_DATA 				(uint8_t)0x01

// commands
#define LCD_CLEARDISPLAY 		(uint8_t)0x01
#define LCD_RETURNHOME 			(uint8_t)0x02

#define LCD_ENTRYMODESET 		(uint8_t)0x04
#define LCD_DISPLAYCONTROL 		(uint8_t)0x08
#define LCD_CURSORSHIFT 		(uint8_t)0x10
#define LCD_FUNCTIONSET 		(uint8_t)0x20
#define LCD_SETCGRAMADDR 		(uint8_t)0x40
#define LCD_SETDDRAMADDR	 	(uint8_t)0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 			(uint8_t)0x00
#define LCD_ENTRYLEFT 			(uint8_t)0x02
#define LCD_ENTRYSHIFTINCREMENT (uint8_t)0x01
#define LCD_ENTRYSHIFTDECREMENT (uint8_t)0x00

// flags for display on/off control
#define LCD_DISPLAYON 			(uint8_t)0x04
#define LCD_DISPLAYOFF 			(uint8_t)0x00
#define LCD_CURSORON 			(uint8_t)0x02
#define LCD_CURSOROFF 			(uint8_t)0x00
#define LCD_BLINKON 			(uint8_t)0x01
#define LCD_BLINKOFF 			(uint8_t)0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 		(uint8_t)0x08
#define LCD_CURSORMOVE 			(uint8_t)0x00
#define LCD_MOVERIGHT 			(uint8_t)0x04
#define LCD_MOVELEFT 			(uint8_t)0x00

// flags for function set
#define LCD_8BITMODE 			(uint8_t)0x10
#define LCD_4BITMODE 			(uint8_t)0x00
#define LCD_2LINE 				(uint8_t)0x08
#define LCD_1LINE 				(uint8_t)0x00
#define LCD_5x10DOTS 			(uint8_t)0x04
#define LCD_5x8DOTS 			(uint8_t)0x00

#define LCD_BACKLIGHT 			(uint8_t)0x08
#define LCD_NOBACKLIGHT 		(uint8_t)0x00
typedef struct
{
	I2C_HandleTypeDef* I2C;
	uint8_t ADDRESS;
	uint8_t COLUMS;
	uint8_t ROWS;
	uint8_t ENTRYMODE;
	uint8_t DISPLAYCTRL;
	uint8_t CURSORSHIFT;
	uint8_t FUNCTIONSET;
	uint8_t BACKLIGHT;
}CLCD_I2C_Name;

extern CLCD_I2C_Name LCD1;

void CLCD_I2C_Init(CLCD_I2C_Name* LCD, I2C_HandleTypeDef* hi2c_CLCD, uint8_t Address, uint8_t Colums, uint8_t Rows);
void CLCD_I2C_SetCursor(CLCD_I2C_Name* LCD, uint8_t Xpos, uint8_t YPos);
void CLCD_I2C_WriteChar(CLCD_I2C_Name* LCD, char character);
void CLCD_I2C_WriteString(CLCD_I2C_Name* LCD, char *String);
void CLCD_I2C_Clear(CLCD_I2C_Name* LCD);
void CLCD_I2C_ReturnHome(CLCD_I2C_Name* LCD);
void CLCD_I2C_CursorOn(CLCD_I2C_Name* LCD);
void CLCD_I2C_CursorOff(CLCD_I2C_Name* LCD);
void CLCD_I2C_BlinkOn(CLCD_I2C_Name* LCD);
void CLCD_I2C_BlinkOff(CLCD_I2C_Name* LCD);
void I2C_LCD_setCursor(uint8_t col, uint8_t row);
void I2C_LCD_SendString(char *str);
#endif
