/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
typedef struct {
    uint8_t isOn;
    uint8_t isDone;
    uint8_t hrLeft;
    uint8_t minLeft;
    uint16_t secLeft;
    uint16_t secLeftTotal;
    uint16_t startTime;
} countDown_t;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void countDown_Set(uint32_t);
void countDown_Tick(uint32_t);
void countDown_Start();
void countDown_Pause();
void countDown_Reset();
void updateDisplayBuffer();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOC
#define SW_Pin GPIO_PIN_5
#define SW_GPIO_Port GPIOA
#define TMCLK_Pin GPIO_PIN_13
#define TMCLK_GPIO_Port GPIOB
#define TMDIO_Pin GPIO_PIN_14
#define TMDIO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define TX_BUFFER 64

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
