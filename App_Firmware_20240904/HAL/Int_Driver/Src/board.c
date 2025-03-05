
#include <board.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "fatfs.h"
#include "i2c.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"
#include "main.h"
#include "usb_host.h"
#include "fatfs.h"
#include "interrupt_process.h"

#include "CLCD_I2C.h"
#include "log.h"


InputInfType InputInf[INPUT_MAX];
InputType Input[INPUT_MAX];
OutputType Output[OUTPUT_MAX];
/**
  * @brief System Clock Configuration
  * @retval None
  */

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 120;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 5;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
}

void HardwareInit(void)
{
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    //MX_USB_HOST_Init();
    //MX_FATFS_Init();
    MX_IWDG_Init();
    MX_USART3_UART_Init();
    MX_TIM2_Init();

    MX_I2C1_Init();
    MX_I2C3_Init();
    MX_USART1_UART_Init();
    MX_TIM4_Init();
	__enable_irq();
	//    MX_TIM9_Init();
    //HAL_TIM_Base_MspInit(&htim1);    
    HAL_TIM_Base_Start_IT(&htim2);
//    HAL_TIM_Base_Start_IT(&htim3);
    HAL_TIM_Base_Start_IT(&htim4);

    HAL_UART_Receive_IT(&huart1, &recUART1, 1);
    HAL_UART_Receive_IT(&huart3, &recUART3, 1);
    CLCD_I2C_Init(&LCD1, &hi2c1, 0x4E, 16, 2);
    CLCD_I2C_Clear(&LCD1);
    CLCD_I2C_SetCursor(&LCD1, 0, 0);
    CLCD_I2C_WriteString(&LCD1,"S_STAND_BY");
}



void OutputInit(void)
{
    Output[0].GPIOx = OUT0_GPIO_Port;
    Output[0].Pinx = OUT0_Pin;

    Output[1].GPIOx = OUT1_GPIO_Port;
    Output[1].Pinx = OUT1_Pin;

    Output[2].GPIOx = OUT2_GPIO_Port;
    Output[2].Pinx = OUT2_Pin;

    Output[3].GPIOx = OUT3_GPIO_Port;
    Output[3].Pinx = OUT3_Pin;

    Output[4].GPIOx = OUT4_GPIO_Port;
    Output[4].Pinx = OUT4_Pin;

    Output[5].GPIOx = OUT5_GPIO_Port;
    Output[5].Pinx = OUT5_Pin;

    Output[6].GPIOx = OUT6_GPIO_Port;
    Output[6].Pinx = OUT6_Pin;

    Output[7].GPIOx = OUT7_GPIO_Port;
    Output[7].Pinx = OUT7_Pin;

    Output[8].GPIOx = OUT8_GPIO_Port;
    Output[8].Pinx = OUT8_Pin;

    Output[9].GPIOx = OUT9_GPIO_Port;
    Output[9].Pinx = OUT9_Pin;

    Output[10].GPIOx = OUT10_GPIO_Port;
    Output[10].Pinx = OUT10_Pin;

    Output[11].GPIOx = OUT11_GPIO_Port;
    Output[11].Pinx = OUT11_Pin;   

    Output[12].GPIOx = OUT12_GPIO_Port;
    Output[12].Pinx = OUT12_Pin;

    Output[13].GPIOx = OUT13_GPIO_Port;
    Output[13].Pinx = OUT13_Pin;

    Output[14].GPIOx = OUT14_GPIO_Port;
    Output[14].Pinx = OUT14_Pin;

    Output[15].GPIOx = OUT15_GPIO_Port;
    Output[15].Pinx = OUT15_Pin;
    
    Output[16].GPIOx = OUT16_GPIO_Port;
    Output[16].Pinx = OUT16_Pin;

//    Output[17].GPIOx = OUT17_GPIO_Port;
//    Output[17].Pinx = OUT17_Pin;

    Output[18].GPIOx = OUT18_GPIO_Port;
    Output[18].Pinx = OUT18_Pin;

    Output[19].GPIOx = OUT19_GPIO_Port;
    Output[19].Pinx = OUT19_Pin;

    Output[20].GPIOx = OUT20_GPIO_Port;
    Output[20].Pinx = OUT20_Pin;

    Output[21].GPIOx = OUT21_GPIO_Port;
    Output[21].Pinx = OUT21_Pin;

    Output[22].GPIOx = OUT22_GPIO_Port;
    Output[22].Pinx = OUT22_Pin;
}

void InputInit(void)
{
    memset(InputInf, 0 , sizeof(InputInf));
    memset(Input, 0, sizeof(Input));

    InputInf[0].GPIOx = GPIOE;
    InputInf[0].Pinx = GPIO_PIN_15;

    InputInf[1].GPIOx = GPIOE;
    InputInf[1].Pinx = GPIO_PIN_14;

    InputInf[2].GPIOx = GPIOE;
    InputInf[2].Pinx = GPIO_PIN_13;
    
    InputInf[3].GPIOx = GPIOE;
    InputInf[3].Pinx = GPIO_PIN_12;

    InputInf[4].GPIOx = GPIOE;
    InputInf[4].Pinx = GPIO_PIN_11;
    
    InputInf[5].GPIOx = GPIOE;
    InputInf[5].Pinx = GPIO_PIN_10;
    
    InputInf[6].GPIOx = GPIOE;
    InputInf[6].Pinx = GPIO_PIN_9;

    InputInf[7].GPIOx = GPIOE;
    InputInf[7].Pinx = GPIO_PIN_8;

    InputInf[8].GPIOx = GPIOE;
    InputInf[8].Pinx = GPIO_PIN_7;

    InputInf[9].GPIOx = GPIOB;
    InputInf[9].Pinx = GPIO_PIN_1;

    InputInf[10].GPIOx = GPIOB;
    InputInf[10].Pinx = GPIO_PIN_0;

    InputInf[11].GPIOx = GPIOC;
    InputInf[11].Pinx = GPIO_PIN_4;

    InputInf[12].GPIOx = GPIOA;
    InputInf[12].Pinx = GPIO_PIN_7;

    InputInf[13].GPIOx = GPIOA;
    InputInf[13].Pinx = GPIO_PIN_6;

    InputInf[14].GPIOx = GPIOA;
    InputInf[14].Pinx = GPIO_PIN_5;

    InputInf[15].GPIOx = GPIOA;
    InputInf[15].Pinx = GPIO_PIN_4;

    InputInf[16].GPIOx = GPIOA;
    InputInf[16].Pinx = GPIO_PIN_3;

    InputInf[17].GPIOx = GPIOA;
    InputInf[17].Pinx = GPIO_PIN_2;

    InputInf[18].GPIOx = GPIOC;
    InputInf[18].Pinx = GPIO_PIN_10;

    InputInf[19].GPIOx = GPIOC;
    InputInf[19].Pinx = GPIO_PIN_11;

    InputInf[20].GPIOx = GPIOC;
    InputInf[20].Pinx = GPIO_PIN_12;
    
    InputInf[21].GPIOx = GPIOD;
    InputInf[21].Pinx = GPIO_PIN_0;
    
    InputInf[22].GPIOx = GPIOD;
    InputInf[22].Pinx = GPIO_PIN_1;
    
    InputInf[23].GPIOx = GPIOD;
    InputInf[23].Pinx = GPIO_PIN_2;
    
    InputInf[24].GPIOx = GPIOD;
    InputInf[24].Pinx = GPIO_PIN_3;
    
    InputInf[25].GPIOx = GPIOD;
    InputInf[25].Pinx = GPIO_PIN_4;
    
    InputInf[26].GPIOx = GPIOD;
    InputInf[26].Pinx = GPIO_PIN_5;
    
    InputInf[27].GPIOx = GPIOD;
    InputInf[27].Pinx = GPIO_PIN_6;

}

void DebugOutput(void)
{
    HAL_GPIO_WritePin(Output[14].GPIOx, Output[14].Pinx, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Output[15].GPIOx, Output[15].Pinx, GPIO_PIN_RESET);

//    while(1)
//    {
//        for (uint8_t i = 0; i < 16; i++)
//        {
//            for (uint8_t j = 0; j < 5; j++)
//            {
//                HAL_GPIO_WritePin(Output[i].GPIO, Output[i].GPIO_Pin, GPIO_PIN_RESET);
//                HAL_Delay(50);
//                HAL_GPIO_WritePin(Output[i].GPIO, Output[i].GPIO_Pin, GPIO_PIN_SET);
//                HAL_Delay(50);
//            }
//        }

//        for (uint8_t i = 0; i < 12; i++)
//        {
//            HAL_GPIO_WritePin(Output[i].GPIO, Output[i].GPIO_Pin, HAL_GPIO_ReadPin(InputInf[i + 16].GPIO, InputInf[i + 16].GPIO_Pin));
//        }
//    }
}

void BuzzerStartUp(void)
{
    for (uint8_t i = 0; i < 10; i++)
    {
        HAL_GPIO_WritePin(Output[16].GPIOx, Output[16].Pinx, GPIO_PIN_SET);
        HAL_Delay(50);
        HAL_GPIO_WritePin(Output[16].GPIOx, Output[16].Pinx, GPIO_PIN_RESET);
        HAL_Delay(50);
    }
}

void BuzzerOn(void)
{
    HAL_GPIO_WritePin(Output[16].GPIOx, Output[16].Pinx, GPIO_PIN_SET);
}

void BuzzerOff(void)
{
    HAL_GPIO_WritePin(Output[16].GPIOx, Output[16].Pinx, GPIO_PIN_RESET);
}

void delay_ms(uint32_t t)
{
    HAL_Delay(t);
}
void BoardInit(void)
{
//    uint8_t a[INPUT_MAX];
	HardwareInit();
    OutputInit();
    InputInit();
    BuzzerOn();
    delay_ms(1000);
    BuzzerOff();
}


void delay_us(uint32_t us)
{
	uint32_t i = 0;
    uint32_t delay = us * 30;
    for (i = 0; i < delay; i++)
    {
        ;
    }
}
