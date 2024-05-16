/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
input_vars input;

volatile char container[1024];
volatile int temp;
volatile int key;
volatile bool logTxDone;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
void HardwareInfo(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)

{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */
    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    MX_USART2_UART_Init();
    MX_IWDG_Init();
    /* USER CODE BEGIN 2 */
    HAL_GPIO_WritePin(Green_GPIO_Port, Green_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Red_GPIO_Port, Red_Pin, GPIO_PIN_SET);
    HardwareInfo();
    VGA_Init(); // Init vgaData_s-Screen
    API_clearscreen(VGA_COLOUR_WHITE);
    API_draw_line(20, 20, 100, 100, VGA_COLOUR_RED, 5, 0);
    API_draw_line(20, -5, 100, 100, VGA_COLOUR_RED, 5, 0);
    API_draw_line(20, 1, 100, 100, VGA_COLOUR_RED, 0, 0);

    API_draw_text(0, 0, VGA_COLOUR_BLACK, "TEST", "Joost", 15, 0, 0);
    API_draw_bitmap(0, 0, 1);
    API_draw_rectangle(0, 0, 20, 20, VGA_COLOUR_CYAN, 0, 0, 0);

    VGA_SetPixel(10, 10, 10);
    VGA_SetPixel(0, 0, 0x00);
    VGA_SetPixel(319, 0, 0x00);
    HAL_GPIO_WritePin(Green_GPIO_Port, Green_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Red_GPIO_Port, Red_Pin, GPIO_PIN_RESET);
    int i;

    for(i = 0; i < LINE_BUFLEN; i++)
    {
        input.line_rx_buffer[i] = 0;
    }

    // Reset some stuff
    input.byte_buffer_rx[0] = 0;
    input.char_counter = 0;
    input.command_execute_flag = FALSE;

    // HAL wants a memory location to store the character it receives from the UART
    // We will pass it an array, but we will not use it. We declare our own variable in the interrupt handler
    // See stm32f4xx_it.c
    HAL_UART_Receive_IT(&huart2, input.byte_buffer_rx, BYTE_BUFLEN);


    // Test to see if the screen reacts to UART
    unsigned char colorTest = TRUE;
    uint32_t timeToFeed = HAL_GetTick() + 100;
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

    while(1)
    {
        if(input.command_execute_flag == TRUE)
        {
            // Do some stuff
            //            printf("yes\n");
            colorTest = ~colorTest; // Toggle screen color
            API_clearscreen(colorTest);

            // When finished reset the flag
            input.command_execute_flag = FALSE;
        }
        if(logTxDone)
        {
            LOG_SendNextLog();
            logTxDone = false;
        }
        if(HAL_GetTick() >= timeToFeed)
        {
            HAL_IWDG_Refresh(&hiwdg); // needs to be fed with in 500ms or device will restart.
            HAL_GPIO_TogglePin(Green_GPIO_Port, Green_Pin);
            timeToFeed = HAL_GetTick() + 499;
        }

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
            |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
    logTxDone = true;
}

void HardwareInfo(void)
{
    LOGH("\n\n\n\n\n");
    LOGH("Device info:");
    LOGH("\tID: 0x%lx;       Rev: 0x%lx ", HAL_GetDEVID(), HAL_GetREVID());
    LOGH("\tCore: Cotrex-m%d; FPU: %s", __CORTEX_M, (__FPU_PRESENT) ? "Yes" : "No");
    LOGH("\tuid: 0x%lx 0x%lx 0x%lX", HAL_GetUIDw0(), HAL_GetUIDw1(), HAL_GetUIDw2());
    LOGH("Clocks:");
    LOGH("\tSystem clock: %lu Hz", HAL_RCC_GetSysClockFreq());
    LOGH("\tAHB clock:    %lu Hz", HAL_RCC_GetHCLKFreq());
    LOGH("\tPCLK1 clock:  %lu Hz", HAL_RCC_GetPCLK1Freq());
    LOGH("\tPCLK2 clock:  %lu Hz", HAL_RCC_GetPCLK2Freq());
#if HAL_RTC_MODULE_ENABLED
    LOGH("\tRTC clock:    Enabled\n");
#else
    LOGH("\tRTC clock:    Disabled\n");
#endif
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
