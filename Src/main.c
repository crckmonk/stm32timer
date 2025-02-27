/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tm1637.h"
#include "stdio.h"
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
uint8_t uCurrentDisplay[4] = {0};
uint8_t utm1637_Segments[8] = {0};
countDown_t cdCountDown;
uint8_t uTxBuffer[TX_BUFFER];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  utm1637_Segments[0] = A_SEG;
  utm1637_Segments[1] = B_SEG;
  utm1637_Segments[2] = C_SEG;
  utm1637_Segments[3] = D_SEG;
  utm1637_Segments[4] = E_SEG;
  utm1637_Segments[5] = F_SEG;
  utm1637_Segments[6] = G_SEG;
  utm1637_Segments[7] = DP_SEG;
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
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
  //countDown_Set(10);
  //tm1637_Display_Handle(3,uCurrentDisplay);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if(TIM3->CNT > 0 && TIM3->CNT < 3600){
    if(TIM3->CNT != cdCountDown.secLeftTotal && !cdCountDown.isOn){
        countDown_Set(TIM3->CNT);       
        sprintf(uTxBuffer, "Encoder Ticks = %u\n\r", ((TIM3->CNT)));
        HAL_UART_Transmit(&huart1, uTxBuffer, sizeof(uTxBuffer), 100);
      }
    }
    if(!HAL_GPIO_ReadPin(SW_GPIO_Port,SW_Pin)){
      if(!cdCountDown.isOn){
        countDown_Start();
      } else {
        countDown_Pause();
      }
    }
    if(cdCountDown.isDone){
      cdCountDown.isDone = cdCountDown.isDone <50?cdCountDown.isDone+1:0;
      HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
      HAL_Delay(100);
    }
   tm1637_Display_Handle(3,uCurrentDisplay);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void countDown_Set(uint32_t sec){
  cdCountDown.secLeftTotal = sec;
  cdCountDown.hrLeft = cdCountDown.secLeftTotal / 3600;
	cdCountDown.minLeft = (cdCountDown.secLeftTotal -(3600 * cdCountDown.hrLeft))/60;
	cdCountDown.secLeft = cdCountDown.secLeftTotal - (3600 * cdCountDown.hrLeft) - (60 * cdCountDown.minLeft);
  cdCountDown.startTime = sec;
  updateDisplayBuffer();
}

void countDown_Tick(uint32_t sec){
  cdCountDown.secLeftTotal -= sec;
  if (cdCountDown.secLeftTotal > 0) {
    cdCountDown.hrLeft = cdCountDown.secLeftTotal / 3600;
	  cdCountDown.minLeft = (cdCountDown.secLeftTotal -(3600 * cdCountDown.hrLeft))/60;
  	cdCountDown.secLeft = cdCountDown.secLeftTotal - (3600 * cdCountDown.hrLeft) - (60 * cdCountDown.minLeft);
    updateDisplayBuffer();
  } else {
    cdCountDown.isOn = 0;
    cdCountDown.isDone = 1;
  }
}

void updateDisplayBuffer(){
  uCurrentDisplay[0] = char2segments((cdCountDown.minLeft / 10) + '0');
  uCurrentDisplay[1] = char2segments((cdCountDown.minLeft % 10) + '0') + DP_SEG;
  uCurrentDisplay[2] = char2segments((cdCountDown.secLeft / 10) + '0');
  uCurrentDisplay[3] = char2segments((cdCountDown.secLeft % 10) + '0');
}
void countDown_Start(){
  cdCountDown.isDone = 0;
  cdCountDown.isOn = 1;
}

void countDown_Pause(){
  cdCountDown.isOn = 0;
}

void countDown_Reset(){
  cdCountDown.isOn = 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  if (htim == &htim2 && cdCountDown.isOn) {
    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
    countDown_Tick(1);
    //
  }
  
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
