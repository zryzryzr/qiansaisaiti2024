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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define adc_num  3
#define adc_cishu 32
#define adc_max adc_num * adc_cishu

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
#define yu_pa0_Pin GPIO_PIN_0
#define yu_pa0_GPIO_Port GPIOF
#define yu_pa1_Pin GPIO_PIN_1
#define yu_pa1_GPIO_Port GPIOF
#define yu_pa2_Pin GPIO_PIN_2
#define yu_pa2_GPIO_Port GPIOF
#define yu_pa3_Pin GPIO_PIN_3
#define yu_pa3_GPIO_Port GPIOF
#define yu_pa4_Pin GPIO_PIN_4
#define yu_pa4_GPIO_Port GPIOF
#define yu_pa5_Pin GPIO_PIN_5
#define yu_pa5_GPIO_Port GPIOF
#define yu_pa6_Pin GPIO_PIN_6
#define yu_pa6_GPIO_Port GPIOF
#define yu_pb5_Pin GPIO_PIN_7
#define yu_pb5_GPIO_Port GPIOF
#define yu_pb6_Pin GPIO_PIN_8
#define yu_pb6_GPIO_Port GPIOF
#define deng_Pin GPIO_PIN_2
#define deng_GPIO_Port GPIOG
#define jiashi_Pin GPIO_PIN_3
#define jiashi_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
