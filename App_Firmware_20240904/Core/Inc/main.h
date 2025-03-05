/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
  #include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OUT20_Pin GPIO_PIN_2
#define OUT20_GPIO_Port GPIOE
#define OUT19_Pin GPIO_PIN_3
#define OUT19_GPIO_Port GPIOE
#define OUT18_Pin GPIO_PIN_4
#define OUT18_GPIO_Port GPIOE
#define OUT16_Pin GPIO_PIN_6
#define OUT16_GPIO_Port GPIOE
#define LED1_Pin GPIO_PIN_1
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_2
#define LED2_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_3
#define LED3_GPIO_Port GPIOC
#define INPUT11_Pin GPIO_PIN_4
#define INPUT11_GPIO_Port GPIOC
#define OUT0_Pin GPIO_PIN_10
#define OUT0_GPIO_Port GPIOB
#define OUT1_Pin GPIO_PIN_11
#define OUT1_GPIO_Port GPIOB
#define OUT2_Pin GPIO_PIN_12
#define OUT2_GPIO_Port GPIOB
#define OUT3_Pin GPIO_PIN_13
#define OUT3_GPIO_Port GPIOB
#define OUT4_Pin GPIO_PIN_14
#define OUT4_GPIO_Port GPIOB
#define OUT5_Pin GPIO_PIN_15
#define OUT5_GPIO_Port GPIOB
#define OUT6_Pin GPIO_PIN_10
#define OUT6_GPIO_Port GPIOD
#define OUT7_Pin GPIO_PIN_11
#define OUT7_GPIO_Port GPIOD
#define OUT8_Pin GPIO_PIN_12
#define OUT8_GPIO_Port GPIOD
#define OUT9_Pin GPIO_PIN_13
#define OUT9_GPIO_Port GPIOD
#define OUT10_Pin GPIO_PIN_14
#define OUT10_GPIO_Port GPIOD
#define OUT11_Pin GPIO_PIN_15
#define OUT11_GPIO_Port GPIOD
#define OUT12_Pin GPIO_PIN_6
#define OUT12_GPIO_Port GPIOC
#define OUT13_Pin GPIO_PIN_7
#define OUT13_GPIO_Port GPIOC
#define OUT14_Pin GPIO_PIN_8
#define OUT14_GPIO_Port GPIOC
#define OUT15_Pin GPIO_PIN_15
#define OUT15_GPIO_Port GPIOA
#define INPUT18_Pin GPIO_PIN_10
#define INPUT18_GPIO_Port GPIOC
#define INPUT19_Pin GPIO_PIN_11
#define INPUT19_GPIO_Port GPIOC
#define INPUT20_Pin GPIO_PIN_12
#define INPUT20_GPIO_Port GPIOC
#define INPUT21_Pin GPIO_PIN_0
#define INPUT21_GPIO_Port GPIOD
#define INPUT22_Pin GPIO_PIN_1
#define INPUT22_GPIO_Port GPIOD
#define INPUT23_Pin GPIO_PIN_2
#define INPUT23_GPIO_Port GPIOD
#define INPUT24_Pin GPIO_PIN_3
#define INPUT24_GPIO_Port GPIOD
#define INPUT25_Pin GPIO_PIN_4
#define INPUT25_GPIO_Port GPIOD
#define INPUT26_Pin GPIO_PIN_5
#define INPUT26_GPIO_Port GPIOD
#define OUT22_Pin GPIO_PIN_0
#define OUT22_GPIO_Port GPIOE
#define OUT21_Pin GPIO_PIN_1
#define OUT21_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */
#define SEND_LOG

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
